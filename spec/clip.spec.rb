require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "clip.pdf" do
        pixel_at(257,354).should_be_of_color 0xffffff
        pixel_at(194,419).should_be_of_color 0x00ff00
        pixel_at(116,496).should_be_of_color 0xffffff
        pixel_at(109,353).should_be_of_color 0xffffff
    end
end
