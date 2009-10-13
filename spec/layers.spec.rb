require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do

  convert_file "layers.pdf" do
    pixel_at(67,134).should_be_of_color 0x00ffff
    pixel_at(299,182).should_be_of_color 0x00ffff
    pixel_at(144,215).should_be_of_color 0x00ffff
    pixel_at(206,203).should_be_of_color 0x00ffff
    pixel_at(243,196).should_be_of_color 0x00ffff
    pixel_at(270,190).should_be_of_color 0x00ffff
    pixel_at(114,220).should_be_of_color 0x00ffff
    pixel_at(406,161).should_be_of_color 0x00ffff
    pixel_at(398,201).should_be_of_color 0x00ffff
    pixel_at(438,211).should_be_of_color 0x0000ff
    pixel_at(499,216).should_be_of_color 0xffff00
    pixel_at(498,223).should_be_of_color 0x00ffff
    pixel_at(515,175).should_be_of_color 0xffff00
    pixel_at(458,185).should_be_of_color 0x0000ff
    pixel_at(541,138).should_be_of_color 0xffff00
    pixel_at(433,158).should_be_of_color 0x0000ff
    area_at(431,155,485,212).should_be_plain_colored
    pixel_at(103,184).should_be_of_color 0x000000
    pixel_at(505,84).should_be_of_color 0xffff00
    pixel_at(314,149).should_be_of_color 0xffffff
  end
end
