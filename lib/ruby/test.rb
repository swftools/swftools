require 'gfx'

pdf = GFX::PDF.new('test.pdf')

pdf.each_page do |page|
    puts "#{page.nr} #{page.width}x#{page.height}"
end

class TestRender < GFX::Device
    def stroke(line, width, color, capstyle, jointstyle, miterlimit)
        p line
    end
end

pdf.page(3).render(TestRender.new)
