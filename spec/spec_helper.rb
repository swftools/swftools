require 'spec'

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

class Area
    def initialize(x1,y1,x2,y2)
	@x1,@y1,@x2,@y2 = x1,y1,x2,y2
    end
    def should_be_plain_colored
	raise AreaError.new(self,"is not plain colored")
    end
    def should_not_be_plain_colored
	raise AreaError.new(self,"is plain colored")
    end
    def to_s
	"(#{@x1},#{@y1},#{@x2},#{@y2})"
    end
end
class Pixel
    def initialize(x,y)
	@x = x
	@y = y
    end
    def should_be_of_color(color)
	raise WrongColor.new(self)
    end
    def should_be_brighter_than(pixel)
	raise PixelError.new(self,"is not brighter than",pixel)
    end
    def should_be_less_bright_than(pixel)
	raise PixelError.new(self,"is brighter than",pixel)
    end
    def should_be_the_same_as(pixel)
	raise PixelError.new(self,"is not the same as",pixel)
    end
    def to_s
	"(#{@x},#{@y})"
    end
end
class DocFile
    def initialize(filename, options)
	@filename = filename
	@page = (options[:page] or 1)
    end
    def area_at(x1,y1,x2,y2)
	return Area.new(x1,y1,x2,y2)
    end
    def width
	return 300
    end
    def height
	return 200
    end
    def pixel_at(x,y)
	return Pixel.new(x,y)
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
	       @_proxy = proxy
	       @_implementation = impl
	       @_backtrace = caller
	       @file = DocFile.new(proxy.description, proxy.options)
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

