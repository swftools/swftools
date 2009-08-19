require 'spec'
require 'rubygems'
require 'RMagick'

class WrongColor < Exception
    def initialize(pixel)
	@pixel = pixel
    end
    def to_s
	"Wrong color at #{@pixel}"
    end
end
class AreaError < Exception
    def initialize(area,problem)
	@area,@problem = area,problem
    end
    def to_s
	"Area at #{@area} #{@problem}"
    end
end
class PixelError < Exception
    def initialize(p1, relation,p2)
	@p1,@p2,@relation = p1,p2,relation
    end
    def to_s
	"Pixel #{@p1} #{@relation} #{@p2}"
    end
end
class ConversionFailed < Exception
    def initialize(output,file)
	@output = output
	@file = file
	@exists = File.exists?(file)
    end
    def to_s
	puts "-"*26+" Conversion failed "+"-"*27
	(puts @output) if @output
	puts "file #{@file} doesn't exist" if not @exists
	puts "-"*72
    end
end

class Area
    def initialize(x1,y1,x2,y2,file)
	@x1,@y1,@x2,@y2,@file = x1,y1,x2,y2,file
    end
    def should_be_plain_colored
	@rgb = @file.get_area(@x1,@y1,@x2,@y2) unless @rgb
	@rgb.minmax == [@rgb[0],@rgb[0]] or raise AreaError.new(self,"is not plain colored")
    end
    def should_not_be_plain_colored
	@rgb = @file.get_area(@x1,@y1,@x2,@y2) unless @rgb
	@rgb.minmax != [@rgb[0],@rgb[0]] or raise AreaError.new(self,"is plain colored")
    end
    def should_contain_text(text)
	text2 = @file.get_text(@x1,@y1,@x2,@y2) 
	text2 == text or raise AreaError.new(self, "doesn't contain text \"#{text}\" (found: \"#{text2}\")")
    end
    def to_s
	"(#{@x1},#{@y1},#{@x2},#{@y2})"
    end
end

def rgb_to_int(rgb)
    # ImageMagick rgb triples are 16 bit
    (rgb.reverse+[0]).map {|c| c>>8}.pack("CCCC").unpack("i")[0]
end

class Pixel
    attr :rgb
    def initialize(x,y,rgb)
	@x,@y,@rgb = x,y,rgb
    end
    def should_be_of_color(color2)
	color1 = rgb_to_int(@rgb)
	color1 == color2 or raise WrongColor.new(self)
    end
    def should_be_brighter_than(pixel)
	gray1 = @rgb.inject(0) {|sum,e| sum+e}
	gray2 = pixel.rgb.inject(0) {|sum,e| sum+e}
	gray1 > gray2 or raise PixelError.new(self,"is not brighter than",pixel)
    end
    def should_be_darker_than(pixel)
	gray1 = @rgb.inject(0) {|sum,e| sum+e}
	gray2 = pixel.rgb.inject(0) {|sum,e| sum+e}
	gray1 < gray2 or raise PixelError.new(self,"is not less bright than",pixel)
    end
    def should_be_the_same_as(pixel)
	@rgb == pixel.rgb or raise PixelError.new(self,"is not the same as",pixel)
    end
    def to_s
	"(#{@x},#{@y})"
    end
end

$tempfiles = []
Kernel.at_exit do
    $tempfiles.each do |file|
	`rm -f #{file}`
    end
end

class DocFile
    def initialize(filename, page)
	@filename = filename
	@page = page
    end
    def convert()
	return if @swfname
	@swfname = @filename.gsub(/.pdf$/i,"")+".swf"
	$tempfiles += [@swfname]
	`pdfinfo #{@filename}` =~ /Page size:\s*([0-9]+) x ([0-9]+) pts/
	width,height = $1,$2
	dpi = (72.0 * 612 / width.to_i).to_i
	output = `pdf2swf -f --flatten -s zoom=#{dpi} -p #{@page} #{@filename} -o #{@swfname} 2>&1`
	#output = `pdf2swf -s zoom=#{dpi} --flatten -p #{@page} #{@filename} -o #{@swfname} 2>&1`
	raise ConversionFailed.new(output,@swfname) unless File.exists?(@swfname)
    end
    def render()
	return if @img
	convert()
	@pngname = @filename.gsub(/.pdf$/i,"")+".png"
	begin
	    output = `swfrender --legacy #{@swfname} -o #{@pngname} 2>&1`
	    raise ConversionFailed.new(output,@pngname) unless File.exists?(@pngname)
	    @img = Magick::Image.read(@pngname).first
	ensure
	    `rm -f #{@pngname}`
	end
    end
    def get_text(x1,y1,x2,y2)
	self.convert()
	#puts "swfstrings -x #{x1} -y #{y1} -W #{x2-x1} -H #{y2-y1} #{@swfname}"
	#puts `swfstrings -x #{x1} -y #{y1} -W #{x2-x1} -H #{y2-y1} #{@swfname}`
	`swfstrings -x #{x1} -y #{y1} -W #{x2-x1} -H #{y2-y1} #{@swfname}`.chomp
    end
    def get_area(x1,y1,x2,y2)
	self.render()
	data = @img.export_pixels(x1, y1, x2-x1, y2-y1, "RGB")
	Array.new(data.size/3) do |i| data.slice(i*3,3) end
    end
    def area_at(x1,y1,x2,y2)
	return Area.new(x1,y1,x2,y2,self)
    end
    def width()
	self.render()
	return @img.columns
    end
    def height()
	self.render()
	return @img.rows
    end
    def pixel_at(x,y)
	self.render()
	data = @img.export_pixels(x, y, 1, 1, "RGB")
	return Pixel.new(x,y,data)
    end
end

module Spec
    module Example
	module ExampleMethods
	   def area_at(x1,y1,x2,y2)
	       @file.area_at(x1,y1,x2,y2)
	   end
	   def width
	       @file.width
	   end
	   def height
	       @file.height
	   end
	   def pixel_at(x,y)
	       @file.pixel_at(x,y)
	   end
	   def initialize(proxy,&impl)

	       # overriding the initialize() function saves us from having to
	       # set up the document in the test. The bad news, however, is that
	       # we have to be very careful not to raise exceptions here-
	       # rspec would miss those.

	       @_proxy = proxy
	       @_implementation = impl
	       @_backtrace = caller
	       @file = DocFile.new(proxy.description, (proxy.options[:page] or 1))
           end
	end
	module ExampleGroupMethods
	    alias :convert_file :example
	    def page(nr)
		nr
	    end
	end
    end
end


