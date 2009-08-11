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
    def initialize(x1,y1,x2,y2,data)
	@x1,@y1,@x2,@y2 = x1,y1,x2,y2
	@rgb = Array.new(data.size/3) do |i| data.slice(i*3,3) end
    end
    def should_be_plain_colored
	@rgb.minmax == [@rgb[0],@rgb[0]] or raise AreaError.new(self,"is not plain colored")
    end
    def should_not_be_plain_colored
	@rgb.minmax != [@rgb[0],@rgb[0]] or raise AreaError.new(self,"is plain colored")
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
class DocFile
    def initialize(filename, page)
	@filename = filename
	@page = page
    end
    def load()
	@swfname = @filename.gsub(/.pdf$/i,"")+".swf"
	@pngname = @filename.gsub(/.pdf$/i,"")+".png"
	begin
	    output = `pdf2swf --flatten -p #{@page} #{@filename} -o #{@swfname} 2>&1`
	    raise ConversionFailed.new(output,@swfname) unless File.exists?(@swfname)
	    output = `swfrender --legacy #{@swfname} -o #{@pngname} 2>&1`
	    raise ConversionFailed.new(output,@pngname) unless File.exists?(@pngname)
	    @img = Magick::Image.read(@pngname).first
	ensure
	    `rm -f #{@swfname}`
	    `rm -f #{@pngname}`
	end
    end
    def area_at(x1,y1,x2,y2)
	self.load()
	data = @img.export_pixels(x1, y1, x2-x1, y2-y1, "RGB")
	return Area.new(x1,y1,x2,y2,data)
    end
    def width()
	self.load()
	return @img.columns
    end
    def height()
	self.load()
	return @img.rows
    end
    def pixel_at(x,y)
	self.load()
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


