require 'spec'
require 'rubygems'
require 'RMagick'

class WrongColor < Exception
  def initialize(pixel, is_color, desired_color)
    @pixel = pixel
    @is_color = is_color
    @desired_color = desired_color
  end
  def to_s
    "Wrong color at %s (is: %06x should be: %06x)" % [@pixel.to_s, @is_color, @desired_color]
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
    @rgb.min == @rgb.max or raise AreaError.new(self,"is not plain colored: colors go from %06x to %06x" % [rgb_to_int(@rgb.min), rgb_to_int(@rgb.max)])
  end
  def should_not_be_plain_colored
    @rgb = @file.get_area(@x1,@y1,@x2,@y2) unless @rgb
    @rgb.min != @rgb.max or raise AreaError.new(self,"is plain colored")
  end
  def should_contain_text(text)
    text2 = @file.get_text(@x1,@y1,@x2,@y2) 
    text2 == text or raise AreaError.new(self, "doesn't contain text \"#{text}\" (found: \"#{text2}\")")
  end
  def should_contain_link(url)
    links = @file.get_links(@x1,@y1,@x2,@y2) 
    (links & [url]).empty? and raise AreaError.new(self, "doesn't contain url \"#{url}\"")
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
    color1 == color2 or raise WrongColor.new(self, color1, color2)
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
    output = `pdf2swf -f -s poly2bitmap -s zoom=#{dpi} -p #{@page} #{@filename} -o #{@swfname} 2>&1`
    #output = `pdf2swf -s zoom=#{dpi} --flatten -p #{@page} #{@filename} -o #{@swfname} 2>&1`
    raise ConversionFailed.new(output,@swfname) unless File.exists?(@swfname)
  end
  def render()
    return if @img
    convert()
    @pngname = @filename.gsub(/.pdf$/i,"")+".png"
    begin
      output = `swfrender #{@swfname} -o #{@pngname} 2>&1`
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
  def get_links(x1,y1,x2,y2)
    self.convert()
	t = `swfdump -a #{@swfname}`
	links = t.scan(/GetUrl2? URL:"([^"]*)"/).inject([]) do |a,u| a + u end
	t.scan(/name "url:([^"]*)"/).inject(links) do |a,u| a + u end
  end
  def get_area(x1,y1,x2,y2)
    self.render()
    data = @img.export_pixels(x1, y1, x2-x1, y2-y1, "RGB")
    Array.new(data.size/3) { |i| data.slice(i*3,3) }
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

module  Spec::Example::ExampleGroupMethods 
  alias :convert_file :example
end

class FileExampleGroup < Spec::Example::ExampleGroup
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
end

Spec::Example::ExampleGroupFactory.default(FileExampleGroup)

Spec::Runner.configure do |config|
  config.before do
    input_file = File.join(File.dirname(__FILE__), description)
    raise "Cannot find input file #{input_file}" unless File.exists?(input_file)
    @file = DocFile.new(input_file, 1)
  end
end
