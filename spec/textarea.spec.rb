require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do

  convert_file "textarea.pdf" do
    area_at(460,94,610,106).should_contain_text '97924ff65f9dfc75450ba'
    area_at(467,373,525,384).should_contain_text '29cf24e47'
    area_at(474,592,543,601).should_contain_text '0afa27099a'
    area_at(59,798,131,808).should_contain_text '4c28e489b4'
  end
end
