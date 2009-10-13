require File.dirname(__FILE__) + '/spec_helper'

describe "pdf conversion" do

  convert_file "textposition.pdf" do
    pixel_at(109,352).should_be_of_color 0xffffff
    pixel_at(115,346).should_be_of_color 0xffffff
    pixel_at(132,368).should_be_of_color 0xffffff
    pixel_at(141,375).should_be_of_color 0xffffff
    pixel_at(149,380).should_be_of_color 0xffffff
    pixel_at(156,386).should_be_of_color 0xffffff
    pixel_at(176,403).should_be_of_color 0xffffff
    pixel_at(189,409).should_be_of_color 0xffffff
    pixel_at(196,421).should_be_of_color 0xffffff
    pixel_at(204,428).should_be_of_color 0xffffff
    pixel_at(16,164).should_be_of_color 0x000000
    pixel_at(28,167).should_be_of_color 0x000000
    pixel_at(30,152).should_be_of_color 0xffffff
    pixel_at(29,139).should_be_of_color 0x000000
    pixel_at(42,194).should_be_of_color 0x000000
    pixel_at(42,182).should_be_of_color 0xffffff
    pixel_at(28,196).should_be_of_color 0x000000
    pixel_at(31,183).should_be_of_color 0xffffff
    pixel_at(50,193).should_be_of_color 0x000000
    pixel_at(52,183).should_be_of_color 0xffffff
    pixel_at(63,194).should_be_of_color 0x000000
    pixel_at(63,178).should_be_of_color 0xffffff
    pixel_at(63,149).should_be_of_color 0xffffff
    pixel_at(62,139).should_be_of_color 0x000000
    pixel_at(99,141).should_be_of_color 0x000000
    pixel_at(98,171).should_be_of_color 0xffffff
    pixel_at(99,194).should_be_of_color 0x000000
    pixel_at(113,168).should_be_of_color 0x000000
    pixel_at(113,152).should_be_of_color 0xffffff
    pixel_at(113,189).should_be_of_color 0xffffff
    pixel_at(126,169).should_be_of_color 0xffffff
    pixel_at(136,167).should_be_of_color 0xffffff
    pixel_at(135,195).should_be_of_color 0x000000
    pixel_at(125,194).should_be_of_color 0x000000
    pixel_at(135,141).should_be_of_color 0x000000
    pixel_at(113,141).should_be_of_color 0x000000
    pixel_at(74,169).should_be_of_color 0xffffff
    pixel_at(310,437).should_be_of_color 0x000000
    pixel_at(313,442).should_be_of_color 0xffffff
    pixel_at(320,430).should_be_of_color 0xffffff
    pixel_at(363,423).should_be_of_color 0xffffff
    pixel_at(383,419).should_be_of_color 0xffffff
    pixel_at(405,413).should_be_of_color 0xffffff
    pixel_at(416,407).should_be_of_color 0x000000
    pixel_at(435,404).should_be_of_color 0x000000
    pixel_at(458,403).should_be_of_color 0xffffff
    pixel_at(474,394).should_be_of_color 0xffffff
    pixel_at(489,385).should_be_of_color 0xffffff
    pixel_at(499,393).should_be_of_color 0xffffff
    pixel_at(525,381).should_be_of_color 0xffffff
    pixel_at(545,376).should_be_of_color 0xffffff
    pixel_at(415,278).should_be_of_color 0xffffff
    pixel_at(409,271).should_be_of_color 0xffffff
    pixel_at(432,256).should_be_of_color 0xffffff
    pixel_at(440,250).should_be_of_color 0xffffff
    pixel_at(449,243).should_be_of_color 0xffffff
    pixel_at(458,237).should_be_of_color 0xffffff
    pixel_at(472,228).should_be_of_color 0xffffff
    pixel_at(476,221).should_be_of_color 0xffffff
    pixel_at(489,214).should_be_of_color 0xffffff
    pixel_at(497,202).should_be_of_color 0xffffff
    pixel_at(504,196).should_be_of_color 0xffffff
    pixel_at(243,262).should_be_of_color 0xffffff
    pixel_at(242,255).should_be_of_color 0xffffff
    pixel_at(253,257).should_be_of_color 0xffffff
    pixel_at(129,370).should_be_of_color 0xffffff
    pixel_at(367,419).should_be_of_color 0xffffff
    pixel_at(387,412).should_be_of_color 0xffffff
    pixel_at(232,188).should_be_of_color 0xffffff
    pixel_at(233,262).should_be_of_color 0xffffff
    pixel_at(207,253).should_be_of_color 0xffffff
    pixel_at(264,254).should_be_of_color 0xffffff
    pixel_at(290,256).should_be_of_color 0xffffff
    pixel_at(303,252).should_be_of_color 0xffffff
    pixel_at(317,256).should_be_of_color 0xffffff
    pixel_at(326,256).should_be_of_color 0xffffff
    pixel_at(317,188).should_be_of_color 0xffffff
    pixel_at(325,193).should_be_of_color 0xffffff
    pixel_at(289,193).should_be_of_color 0xffffff
    pixel_at(252,194).should_be_of_color 0xffffff
    pixel_at(206,197).should_be_of_color 0xffffff
    pixel_at(206,188).should_be_of_color 0xffffff
    pixel_at(222,123).should_be_of_color 0xffffff
    pixel_at(231,118).should_be_of_color 0xffffff
    pixel_at(245,128).should_be_of_color 0xffffff
    pixel_at(260,124).should_be_of_color 0xffffff
    pixel_at(285,124).should_be_of_color 0xffffff
    pixel_at(295,123).should_be_of_color 0xffffff
    pixel_at(305,118).should_be_of_color 0xffffff
    pixel_at(316,118).should_be_of_color 0xffffff
    pixel_at(343,128).should_be_of_color 0xffffff
    pixel_at(343,118).should_be_of_color 0xffffff
    pixel_at(303,198).should_be_of_color 0xffffff
    pixel_at(43,136).should_be_of_color 0xffffff
    pixel_at(53,135).should_be_of_color 0xffffff
    area_at(17,177,76,197).should_not_be_plain_colored
    area_at(74,134,141,162).should_not_be_plain_colored
    area_at(77,173,141,196).should_not_be_plain_colored
    area_at(219,119,346,128).should_not_be_plain_colored
    area_at(110,347,117,354).should_not_be_plain_colored
    area_at(129,366,138,373).should_not_be_plain_colored
    area_at(139,373,149,381).should_not_be_plain_colored
    area_at(166,396,176,403).should_not_be_plain_colored
    area_at(187,413,197,420).should_not_be_plain_colored
    area_at(199,422,208,429).should_not_be_plain_colored
    area_at(317,430,347,434).should_not_be_plain_colored
    area_at(353,420,382,425).should_not_be_plain_colored
    area_at(396,408,427,414).should_not_be_plain_colored
    area_at(430,400,461,404).should_not_be_plain_colored
    area_at(478,389,503,395).should_not_be_plain_colored
    area_at(513,381,539,385).should_not_be_plain_colored
    area_at(203,189,330,197).should_not_be_plain_colored
    area_at(204,253,331,261).should_not_be_plain_colored
    area_at(410,271,415,277).should_not_be_plain_colored
    area_at(423,261,428,266).should_not_be_plain_colored
    area_at(440,246,445,257).should_not_be_plain_colored
    area_at(460,228,465,235).should_not_be_plain_colored
    area_at(475,217,483,223).should_not_be_plain_colored
    area_at(489,206,493,212).should_not_be_plain_colored
    area_at(502,197,505,202).should_not_be_plain_colored
  end
end
