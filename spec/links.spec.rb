require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "links.pdf" do
        area_at(0,0,200,200).should_contain_link("http://www.swftools.org")
        area_at(0,0,200,200).should_contain_link("http://www.pdf2swf.org")
        area_at(0,0,200,200).should_contain_link("http://www.quiss.org")
    end
end
