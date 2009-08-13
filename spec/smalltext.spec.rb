require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "smalltext.pdf" do
        area_at(37,93,44,103).should_not_be_plain_colored
        area_at(77,94,86,104).should_not_be_plain_colored
        area_at(117,94,123,104).should_not_be_plain_colored
        area_at(157,94,165,103).should_not_be_plain_colored
        area_at(37,139,45,155).should_not_be_plain_colored
        area_at(76,139,85,154).should_not_be_plain_colored
        area_at(116,142,125,152).should_not_be_plain_colored
        area_at(155,145,167,151).should_not_be_plain_colored
        area_at(27,110,180,115).should_be_plain_colored
    end
end
