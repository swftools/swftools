require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do

  convert_file "textselectspaces.pdf" do
        area_at(32,22,257,46).should_contain_text 'The quick brown fox'
        area_at(32,53,226,77).should_contain_text 'The quick brown fox'
        area_at(31,84,198,108).should_contain_text 'Thequickbrownfox'
        area_at(33,111,423,137).should_contain_text 'Thequickbrownfox'
        area_at(35,141,455,167).should_contain_text 'The quick brown fox'
    end
end
