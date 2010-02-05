require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "transparency.pdf" do
        pixel_at(174,135).should_be_of_color 0xff0000
        pixel_at(167,112).should_be_of_color 0x000000
        pixel_at(172,75).should_be_of_color 0x000000
        pixel_at(199,108).should_be_of_color 0x000000
        pixel_at(234,141).should_be_of_color 0x000000
        pixel_at(202,142).should_be_of_color 0x0000ff
        pixel_at(233,111).should_be_of_color 0x00ffff
        pixel_at(233,71).should_be_of_color 0xffff00
        pixel_at(199,71).should_be_of_color 0x00ff00
        pixel_at(594,277).should_be_brighter_than pixel_at(439,279)
        pixel_at(552,284).should_be_brighter_than pixel_at(361,285)
        pixel_at(474,294).should_be_brighter_than pixel_at(325,277)
        pixel_at(283,276).should_be_brighter_than pixel_at(94,277)
        area_at(86,290,107,301).should_not_be_plain_colored
        area_at(158,287,186,300).should_not_be_plain_colored
        area_at(234,288,262,300).should_not_be_plain_colored
        area_at(312,287,337,300).should_not_be_plain_colored
        area_at(162,438,244,455).should_not_be_plain_colored
        pixel_at(201,400).should_be_brighter_than pixel_at(174,345)
        pixel_at(299,477).should_be_brighter_than pixel_at(237,555)
        area_at(407,400,435,422).should_be_plain_colored
    end
end
