require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
  
  convert_file "transpstack.pdf" do
        pixel_at(15,15).should_be_of_color 0xffffff
        pixel_at(4,50).should_be_of_color 0xff0000
        pixel_at(21,52).should_be_of_color 0x00ff00
        pixel_at(49,51).should_be_of_color 0x0000ff
        pixel_at(80,49).should_be_of_color 0x0000ff
        pixel_at(23,77).should_be_of_color 0x00ff00
        pixel_at(70,20).should_be_of_color 0x0000ff
    end
end
