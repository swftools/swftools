require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
  
  convert_file "miterlimit.pdf" do
    pixel_at(451,153).should_be_of_color 0x999999
    pixel_at(492,154).should_be_of_color 0x666666
    pixel_at(523,153).should_be_of_color 0x666666
    pixel_at(386,151).should_be_of_color 0x666666
    pixel_at(86,65).should_be_of_color 0x666666
    pixel_at(80,77).should_be_of_color 0x999999
    pixel_at(77,86).should_be_of_color 0x666666
    pixel_at(474,380).should_be_of_color 0xffffff
    pixel_at(492,380).should_be_of_color 0xffffff
    pixel_at(452,381).should_be_of_color 0x999999
    pixel_at(450,369).should_be_of_color 0x666666
    pixel_at(376,382).should_be_of_color 0x666666
    pixel_at(89,319).should_be_of_color 0x666666
    pixel_at(119,314).should_be_of_color 0x999999
    pixel_at(142,305).should_be_of_color 0x666666
  end
end
