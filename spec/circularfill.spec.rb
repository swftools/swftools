require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do

  convert_file "circularfill.pdf" do
    pixel_at(194,224).should_be_of_color 0xffffff
    pixel_at(169,225).should_be_of_color 0xff0000
    pixel_at(138,225).should_be_of_color 0xffffff
    pixel_at(72,162).should_be_of_color 0xff0000
    pixel_at(195,132).should_be_of_color 0xff0000
    pixel_at(69,132).should_be_of_color 0xffffff
    pixel_at(310,289).should_be_of_color 0xff0000
    pixel_at(287,200).should_be_of_color 0xff0000
    pixel_at(309,161).should_be_of_color 0xff0000
    pixel_at(287,161).should_be_of_color 0xff0000
    pixel_at(109,336).should_be_of_color 0xff0000
    pixel_at(310,306).should_be_of_color 0xff0000
    pixel_at(282,340).should_be_of_color 0xff0000
    pixel_at(409,252).should_be_of_color 0xff0000
    pixel_at(139,310).should_be_of_color 0xffffff
    pixel_at(312,343).should_be_of_color 0xffffff
    pixel_at(311,248).should_be_of_color 0xff0000
    pixel_at(102,228).should_be_of_color 0xff0000
    pixel_at(374,281).should_be_of_color 0xff0000
    pixel_at(167,281).should_be_of_color 0xff0000
    pixel_at(409,285).should_be_of_color 0xffffff
    pixel_at(166,190).should_be_of_color 0xffffff
    pixel_at(138,191).should_be_of_color 0xff0000
  end
end
