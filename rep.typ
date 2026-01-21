
#import "typst_utils/utils.typ": *

#set text(..default_text_parm)
#set par(..default_par_parm)
#set table(stroke: none)
#set pagebreak(weak: true)

#set image(..default_image_parm)
#set grid(..default_grid_parm)
#show bibliography: set grid(..default_bib_grid_parm)

#show figure.where(kind: image): set figure(..default_FOI_parm)
#show figure.where(kind: table): set figure(..default_FOT_parm)
#show figure.caption: set text(size: font_size_zh.WuHao)
#show figure.where(kind: table): set figure.caption(position: top)

#show heading.where(level: 1): set heading(numbering: "1")
#show heading.where(level: 2): set heading(numbering: "1.1")
#show heading.where(level: 3): set heading(numbering: "1.1.1")

//page style
#set page(
  footer: [
    #set align(center)
    #context counter(page).display("1")
  ],
)

// bib upercase style; disable auto-generated footnote
#set footnote(numbering: "[1]")
#set footnote.entry(separator: none)
#show footnote.entry: hide

#show math.equation: set text(font: ("New Computer Modern Math", "SimSun"))

#show heading.where(level: 1): set heading(numbering: none)

#outline(title: [
  #grid(align: center, columns: 1fr, "目录")
])

#show heading.where(level: 1): set heading(numbering: "1")

#pagebreak()

#Section("问题描述")

#FLI() 问题来自2023 年高教社杯全国大学生数学建模竞赛A题 定日镜场的优化设计（第三问)

#FLI() 在塔式太阳能光热发电站中，定日镜场由众多定日镜组成。这些定日镜随着太阳的运动而自动调整位置，以精确地将太阳光线汇聚到吸收塔上。我们可以调整定日镜的位置和尺寸，以最大程度地提高光热发电的效率。

#FLI() 定日镜的底座包括水平转轴和纵向转轴，水平转轴上安装了平面反射镜。其中，水平转轴可以控制反射镜的俯仰角，纵向转轴可以控制反射镜的方位角，随着太阳位置的改变，定日镜的法向也跟着改变，使得定日镜中心反射的锥形太阳光线可以指向集热器中心。定日镜场内排列了大量的定日镜以及一个吸收塔，集热器位于吸收塔的顶端，其以热能的形式储存吸收得到的太阳能，然后再进一步将热能转换成电能。

#FLI() 问题要求在中心位于东经 98.5 ∘，北纬 39.4 ∘，海拔 3000 m，半径 350 m 的圆形区域内建设一个圆形定日镜场。以圆形区域中心为原点，正东方向为x轴正向，正北方向为y轴正向，垂直于地面向上方向为 z 轴正向建立坐标系，称为镜场坐标系。为简化计算，本问题中所有“年均”指标的计算时点均为当地时间每月 21 日 9:00、10:30、12:00、13:30、15:00。

#FLI() 该题第三问是对前2问的扩展, 额定功率仍为60兆瓦（MW）。设计目标是找到吸收塔的位置坐标、各定日镜尺寸、安装高度、定日镜数目以及定日镜的位置坐标，以使得定日镜场能够在达到额定功率的条件下，尽可能地提高单位镜面面积年平均输出热功率。

#Section("数学建模")

#SubSection("符号约定")

#FLI() 本模型中的符号约定如@nomenclature 所示 (注: 关于优化方法涉及的符号不在此给出; 对所有脚标i成立的式子省略了脚标)。

#[
  #set align(center)
  #set text(top-edge: 0.5em, bottom-edge: 0.5em)
  #figure(
    table(
      columns: (1fr, 3fr, 1fr, 4fr),
      table.hline(y: 0),
      table.hline(y: 1, stroke: 0.5pt),
      table.hline(y: 6),
      [符号], [说明], [符号], [说明],
      [$D$], [镜间最小间距], [$r_i$], [围绕集热塔第i个环的半径],
      [$Delta theta_i$], [第i个环中相邻镜的夹角], [$x_i,y_i,h_i$], [镜i在镜场坐标系下的坐标],
      [$a_i,b_i$], [镜i的长和宽], [$R_(min),R_(max)$], [镜距离集热塔最近/最远距离],
      [$x_c,y_c$], [集热塔的坐标], [$h_(c 1),h_(c 2),r_c$], [集热塔柱高,集热部分的高度和半径],
      [$Omega$], [太阳的角直径],
      [$arrow(d)$], [镜面中心到集热中心的向量], [$arrow(L)$], [太阳光的主方向], [], [],
      [$eta_(cos,i)$], [镜i的余弦效率], [$eta_(a,i)$], [镜i的大气散射效率],
      [$eta_("trunc",i)$], [镜i的截断效率], [$eta_(s,i)$], [镜i的阴影效率],
    ),
    caption: "符号约定",
  )<nomenclature>
]

#SubSection("模型的建立")

#FLI() 本报告在计算单位镜面积年平均输出功率时, 需要对定日镜场的三维场景和光的传播进行建模和计算, 从镜坐标系的建立, 光学效率的计算和单位镜面积年平均功率的整理3个部分进行建模。

#SubSub("镜坐标系的建立")

#FLI() 对每个镜面建立坐标系时分析其光照性质的前提。建立镜坐标系是根据 $(x_c,y_c,h_(c 1),h_(c 2),x_i,y_i,h_i)$ 确定镜i的法向单位向量 $arrow(N)$, 长和宽的延伸方向 $arrow(n)_3, arrow(n)_3$, 镜坐标系由这3个向量张成。

#FLI() 要使截断效率最小, 反射光的主方向应指向集热塔集热部分的中心, 也即镜面中心到该点的向量:

$
  arrow(d) = (x_c,y_c,h_(c 1)+ 1/2 h_(c 2))^T - (x_i, y_i, h_i)^T
$

#FLI() 给定 $arrow(L)$ 和 $arrow(d)$, 镜的法向 $arrow(N)$ 由反射定律得出:

$
  arrow(N) = "norm" ("norm" (arrow(L)) = "norm" (arrow(d)) )
$
其中 $"norm" (arrow(x)) = arrow(x) / (|arrow(x)|)$ 表示向量的归一化。

#FLI() 由于阴影来自光路上的遮挡, 让镜的长边(a)垂直于光路的主方向, 又因与 $arrow(N)$ 的垂直关系,
得到 $arrow(n)_2 = "norm" (d_x,d_y,0)$, 进而 $arrow(n)_3 = arrow(N) times arrow(n)_2$。显然三者组成单位正交基。

#FLI() 在镜的坐标系下, 镜的四个顶点为 $(0,plus.minus 1/2 a, plus.minus 1/2 b)$。

#SubSub("各项光学效率的计算")

#FLI() (1) 余弦效率来自镜面法向与太阳光方向的夹角, 由于太阳角直径 $$, 太阳光的发散性弱, 在计算余弦效率时不考虑 $$ 的影响。计算公式为

$
  eta_(cos) = "norm" (arrow(d)) dot arrow(N)
$

#FLI() (2) 大气散射效率低计算已在题目中给出, 为

$
  eta_(a)=0.99321-0.000117 |arrow(d)| + 1.97 times 10^(-8) |arrow(d)|^2
$

#FLI() (3) 截断效率来自太阳光的发散性。考虑从距离 $arrow(d)$ 处向集热部分反射来的太阳光, 沿光线方向在集热中心处的平面内, 太阳光发散为直径 $Omega |arrow(d)|$ 的光斑, 集热区域在这一面上的投影近似为 $h_(c 2) (d_x^2 + d_y^2)/(|arrow(d)|)$ 的矩形, 考虑单位立体角能量近似均匀分布, 则该点的截断效率为 $eta_("trunc")(alpha,beta)("光斑和投影重叠面积")/("光斑面积")$ 。若令对地面的水平方向为该平面的x方向, 则镜面上一点 $(0,alpha,beta)$ 在该平面上的偏移为 $(alpha, beta eta_(cos))$, 由于该偏移与集热塔半径 $r_c$ 没有数量级的差异, 因此不可忽略。同时, 随镜面各点的阴影效率可能随点的位置 $(0,alpha,beta)$ 变化, 其相互作用也不能忽略。综合考虑, 截断效率应当与阴影效率视作一个整体给出, 并对镜面上各点统计求和。

#FLI() (4) 阴影效率来自入射光被镜遮挡, 入射光被塔遮挡和反射光被镜遮挡。由于采样点时间固定, 塔的阴影也相对固定, 考虑塔的遮挡可能结果都鲁棒性不足, 因此不纳入考虑; 同时考虑阴影只来自邻近的镜并且邻近的镜张开的坐标近似一致。根据几何关系, 镜i上一点 $(0,alpha,beta)$ 在邻近的镜j坐标下的位置为 $(gamma', alpha',beta') = (0,alpha,beta)+(arrow(p)dot arrow(N),arrow(p)dot arrow(n)_2, arrow(p) dot arrow(n)_3)$ 其中 $arrow(p) = (x_i - x_j , y_i - y_j, h_i - h_j)^T$, 若 $gamma>0$ 则说明该点在镜j的上方, 可能带来阴影; 进一步考虑入射光和反射光方向的斜投影 $(alpha'',beta'') = (alpha' plus.minus gamma' sqrt(1-eta_(cos)^2))$, 投影得到的区域仍为矩形。对镜上一点 $(alpha,beta)$ 处的阴影效率为

$
  eta_("trunc") (alpha,beta) =cases(1 quad "不在阴影中", 0 quad "在阴影中")
$

#FLI() 镜的截断效率和阴影效率为对镜上各点的统计求和:

$
  eta_s &= integral.double_(|alpha|<=a/2,|beta|<=b/2) eta_s (alpha,beta) "d" alpha "d" beta\
  eta_s eta_("trunc") &= integral.double_(|alpha|<=a/2,|beta|<=b/2) eta_s (alpha,beta) eta_("trunc") (alpha,beta) "d" alpha "d" beta
$

// 模型的边界条件?

#SubSub("单位镜面积年平均功率的计算")

#FLI() 总面积 $A=sum_i a_i b_i$, 年平均功率 $W=...$, 优化目标为镜单位面积的年平均功率 $f=$, 考虑边界条件, 得到优化目标

$
  min_(x_i, y_i, h_i, a_i,b_i, x_c, y_c) f\
  "s.t." cases(
    (x_i-x_j)^2 + (y_i-y_j)^2 >= a_i^2 quad forall i !=j,
    (x_i-x_c)^2 + (y_i - y_c)^2 >= R_(min)^2=100^2 quad forall i,
    x_c^2 + y_c^2 <= 350^2,
    x_i^2 + y_i^2 <= 350^2 quad forall i,
    a_i >= b_i quad forall i,
    h_i >= 1/2 b_i quad forall i
  )
$

#SubSection("优化目标的简化")

#SubSub("参数范围限制")

#FLI() (1) 集热塔位置限制: 由于对称性, $x_c=0$; 由于该纬度位于北半球, 镜位于北侧更有利于余弦效率, $y_c<0$。

#FLI() (2) 最远距离限制: 初步实验下发现在给定范围内(半径350m的圆)年平均功率不会超过限制(60kW)太多, 故令 $R_(max)->infinity$ 舍弃对该参数的优化。

#SubSub("蒙特卡洛方法计算截断效率和阴影效率")

#FLI() 计算截断效率和阴影效率设计到对积分的近似, 故采用蒙特卡洛方法进行随机采样。包括2个部分: (1) 在镜面上随机选择点: 将镜面划分为4x4的网格, 在每一网格中随机选取一点; (2) 在光斑上随机选择点: 在光斑平面上围绕 $$ 中心随机选取7个点。

#SubSub("通过集热器参数生成镜的坐标")

#FLI() 镜分布的稀疏程度受镜间距和镜的尺寸共同决定, 因此在给定了镜间距 $D$ 和最远距离 $R_(max)$ 下可以通过一个最密排布流程得到所有镜的位置

//

#SubSub("插值方法生成镜的参数")

//

#Section("优化算法")

#SubSection("粒子群算法优化单位镜面积功率")

#FLI() 粒子群算法是一种启发式的群体智能算法。对每一个个体 $arrow(p)_i$, 粒子群算法的更新方法为 $arrow(p)_i <- w dot arrow(p)_i + c_1 r_1 (arrow(p)^* - arrow(p)_i) + c_2 r_2 (arrow(p)_i^* - arrow(p)_i)$, 其中 $w$ 为惯性参数, $c_1, c_2$ 为学习参数, $r_1, r_2$ 为随机值, $arrow(p)^*$ 为群体的历史最佳位置, $arrow(p)_i^*$ 为该个体的历史最佳位置。本报告从零开始实现了针对定日场单位镜面积年均功率目标的粒子群算法。

#SubSection("惩罚函数法控制总功率")

#FLI() 最总功率要求, 设计惩罚函数 $g(W) = max(0, 60000-W)$, 实际优化目标变为 $min g(W) + f$; 考虑到粒子群算法依赖于历史最优值, 一个权重递增的惩罚函数优化虽然利于最优值的搜索, 但是会导致历史最佳值不可靠, 远离可行域, 为此对惩罚函数采用恒定权重。

#Section("结果展示和分析")

#Section("心得体会")

#Section("附录")

#SubSection("分工")
