require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "pattern.pdf" do
        area_at(425,190,500,224).should_not_be_plain_colored
        area_at(517,192,583,225).should_be_plain_colored
    end
end
