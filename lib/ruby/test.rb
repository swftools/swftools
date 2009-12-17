require 'gfx'

pdf = GFX::PDF.new('test.pdf')

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
        puts "startclip(#{line})"
    end
    def endclip()
        puts "endclip()"
    end
    def stroke(line, width, color, cap_style, joint_style, miterLimit)
        puts "stroke(#{line}, #{width}, #{color}, #{cap_style}, #{joint_style}, #{miterLimit})"
    end
    def fill(line, color)
        puts "fill(#{line}, #{color})"
    end
    def fillbitmap(line, img, imgcoord2devcoord, cxform)
        puts "fillbitmap(#{line}, #{img}, #{imgcoord2devcoord}, #{cxform})"
    end
    def fillgradient(dev, line, gradient, type, gradcoord2devcoord)
        puts "fillgradient(#{line}, #{gradient}, #{type}, #{gradcoord2devcoord})"
    end
    def addfont(font)
        puts "addfont(#{font})"
    end
    def drawchar(font, glyph, color, matrix)
        puts "drawchar(#{font}, #{glyph}, #{color}, #{matrix})"
    end
    def drawlink(line, action)
        puts "drawchar(#{line}, #{action})"
    end
end

r = TestRender.new
pdf.each_page do |page|
    puts "#{page.nr} #{page.width}x#{page.height}"
    page.render(r)
end

