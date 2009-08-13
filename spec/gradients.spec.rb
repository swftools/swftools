require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do
    convert_file "gradients.pdf" do
        pixel_at(269,324).should_be_brighter_than pixel_at(155,438)
        pixel_at(261,388).should_be_brighter_than pixel_at(209,434)
        pixel_at(195,329).should_be_brighter_than pixel_at(164,367)
        pixel_at(33,318).should_be_brighter_than pixel_at(92,381)
        pixel_at(168,310).should_be_brighter_than pixel_at(108,381)
        pixel_at(166,455).should_be_brighter_than pixel_at(109,401)
        pixel_at(25,454).should_be_brighter_than pixel_at(89,396)
        pixel_at(258,210).should_be_brighter_than pixel_at(90,215)
        pixel_at(259,225).should_be_brighter_than pixel_at(95,227)
        pixel_at(257,240).should_be_brighter_than pixel_at(102,240)
        pixel_at(359,121).should_be_brighter_than pixel_at(360,270)
        pixel_at(383,119).should_be_brighter_than pixel_at(385,181)
        pixel_at(373,167).should_be_brighter_than pixel_at(373,239)
        pixel_at(355,409).should_be_brighter_than pixel_at(356,349)
        pixel_at(370,377).should_be_brighter_than pixel_at(369,316)
        pixel_at(382,333).should_be_brighter_than pixel_at(383,286)
        pixel_at(422,115).should_be_brighter_than pixel_at(422,411)
        pixel_at(437,190).should_be_brighter_than pixel_at(436,318)
        pixel_at(447,276).should_be_brighter_than pixel_at(446,413)
        pixel_at(413,101).should_be_the_same_as pixel_at(448,101)
        pixel_at(412,166).should_be_the_same_as pixel_at(449,166)
        pixel_at(412,246).should_be_the_same_as pixel_at(450,246)
        pixel_at(413,332).should_be_the_same_as pixel_at(450,332)
        pixel_at(414,400).should_be_the_same_as pixel_at(451,400)
        pixel_at(87,245).should_be_the_same_as pixel_at(87,212)
        pixel_at(143,245).should_be_the_same_as pixel_at(143,209)
        pixel_at(189,246).should_be_the_same_as pixel_at(189,210)
        pixel_at(236,244).should_be_the_same_as pixel_at(236,208)
    end
end
