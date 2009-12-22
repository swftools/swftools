require 'gfx'

pdf = GFX::PDF.new('file.pdf')

class TestRender < GFX::Device
    def startpage(width,height)
        puts "startpage(#{width},#{height})"
    end
    def endpage()
        puts "endpage()"
    end
    def setparameter(key,value)
        puts "setparameter(#{key},#{value})"
    end
    def startclip(line)
        puts "startclip(#{line.inspect})"
    end
    def endclip()
        puts "endclip()"
    end
    def stroke(line, width, color, cap_style, joint_style, miterLimit)
        puts "stroke(#{line.inspect}, #{width}, #{color.inspect}, #{cap_style}, #{joint_style}, #{miterLimit})"
    end
    def fill(line, color)
        puts "fill(#{line.inspect}, #{color.inspect})"
    end
    def fillbitmap(line, img, imgcoord2devcoord, cxform)
        puts "fillbitmap(#{line.inspect}, #{img}, #{imgcoord2devcoord}, #{cxform})"
    end
    def fillgradient(dev, line, gradient, type, gradcoord2devcoord)
        puts "fillgradient(#{line.inspect}, #{gradient}, #{type}, #{gradcoord2devcoord})"
    end
    def addfont(font)
        puts "addfont(#{font.name})"
    end
    def drawchar(font, glyph, color, matrix)
        puts "drawchar(#{font.name}, #{glyph}, #{color.inspect}, #{matrix.inspect})"
    end
    def drawlink(line, action)
        puts "drawchar(#{line.inspect}, #{action})"
    end
end

r = TestRender.new
pdf.each_page do |page|
    puts "#{page.nr} #{page.width}x#{page.height}"
    page.render(r)
end

