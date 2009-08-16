require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "simpletext.pdf" do
        pixel_at(109,133).should_be_of_color 0x000000
        pixel_at(154,139).should_be_brighter_than pixel_at(151,139)
        pixel_at(53,136).should_be_darker_than pixel_at(58,166)
        pixel_at(97,164).should_be_the_same_as pixel_at(208,256)
        area_at(215,80,333,156).should_be_plain_colored
        area_at(46,126,180,158).should_not_be_plain_colored
        area_at(38,118,170,152).should_contain_text 'Hello World'
    end
end
