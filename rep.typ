
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

// bib upercase style; disable auto-generated footnote
#set footnote(numbering: "[1]")
#set footnote.entry(separator: none)
#show footnote.entry: hide

#show math.equation: set text(font: ("New Computer Modern Math", "SimSun"))

#show heading.where(level: 1): set heading(numbering: none)

#outline(title: [
  #grid(align: center, columns: 1fr, "目录")
])


//page style
#counter(page).update(0)
#set page(
  footer: [
    #set align(center)
    #context counter(page).display("1")
  ],
)
#show heading.where(level: 1): set heading(numbering: "1")

#pagebreak()

#set image(height: 7.5cm, width: 10cm)

#Section("问题描述")

#FLI() 问题来自2023 年高教社杯全国大学生数学建模竞赛A题 定日镜场的优化设计（第三问)

#FLI() 在塔式太阳能光热发电站中，定日镜场由众多定日镜组成。这些定日镜随着太阳的运动而自动调整位置，以精确地将太阳光线汇聚到吸收塔上。我们可以调整定日镜的位置和尺寸，以最大程度地提高光热发电的效率。

#FLI() 定日镜的底座包括水平转轴和纵向转轴，水平转轴上安装了平面反射镜。其中，水平转轴可以控制反射镜的俯仰角，纵向转轴可以控制反射镜的方位角，随着太阳位置的改变，定日镜的法向也跟着改变，使得定日镜中心反射的锥形太阳光线可以指向集热器中心。定日镜场内排列了大量的定日镜以及一个吸收塔，集热器位于吸收塔的顶端，其以热能的形式储存吸收得到的太阳能，然后再进一步将热能转换成电能。

#FLI() 问题要求在中心位于北纬 39.4°，海拔 3000 m，半径 350 m 的圆形区域内建设一个圆形定日镜场。以圆形区域中心为原点，正东方向为x轴正向，正北方向为y轴正向，垂直于地面向上方向为 z 轴正向建立坐标系，称为镜场坐标系。问题中所有“年均”指标的计算时点均为当地时间每月 21 日 9:00、10:30、12:00、13:30、15:00。

#FLI() 此外, 问题还要求吸收塔高度 80 m，集热器高 8 m、直径 7 m 。吸收塔周围 100 m 范围内不安装定日镜，定日镜的形状为平面矩形，其上下两条边始终平行于地面，通常镜面宽度不小于镜面高度。镜面边长在 2 m 至 8 m 之间，安装高度在 2 m 至 6 m 之间，安装高度必须保证镜面在绕水平转轴旋转时不会触及
地面。要求相邻定日镜底座中心之间的距离比镜面宽度多 5 m 以上。

#FLI() 该题第三问是对前2问的扩展, 额定功率为60兆瓦（MW）。设计目标是找到吸收塔的位置坐标、各定日镜尺寸、安装高度、定日镜数目以及定日镜的位置坐标，以使得定日镜场能够在达到额定功率的条件下，尽可能地提高单位镜面面积年平均输出热功率。

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
      table.hline(y: 10),
      [符号], [说明], [符号], [说明],
      [$D$], [镜间最小间距], [$r_i$], [围绕集热塔第i个环的半径],
      [$Delta theta_i$], [第i个环中相邻镜的夹角], [$x_i,y_i,h_i$], [镜i在镜场坐标系下的坐标],
      [$a_i,b_i$], [镜i的长和宽], [$R_(min),R_(max)$], [镜距离集热塔最近/最远距离],
      [$x_c,y_c$], [集热塔的坐标], [$h_(c 1),h_(c 2),r_c$], [集热塔柱高,集热部分的高度和半径],
      [$Omega$], [太阳的角直径],
      [$arrow(d)$], [镜面中心到集热中心的向量],
      [$eta_(cos,i)$], [镜i的余弦效率], [$eta_(a,i)$], [镜i的大气散射效率],
      [$eta_("trunc",i)$], [镜i的截断效率], [$eta_(s,i)$], [镜i的阴影效率],
      [$arrow(L)$], [太阳光的主方向], [$eta_(r,i)$], [镜i的镜面反射率],
      [$phi$], [太阳的仰角], [$theta$], [太阳的方位角],
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

#FLI() (1) 余弦效率来自镜面法向与太阳光方向的夹角, 由于太阳角直径 $Omega$, 太阳光的发散性弱, 在计算余弦效率时不考虑其影响。计算公式为

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

#FLI() (5) 镜面反射效率根据题目要求取固定值 $eta_r = 0.92$。

// 模型的边界条件?

#SubSub("单位镜面积年平均功率的计算")

#FLI() 题目中已给出了根据日期Date, 纬度Lat和当前时间ST计算太阳仰角 $phi$ 和方位角 $theta$ 的方法, 以及根据海拔和太阳仰角计算法向辐照度DNI的方法, 在此不赘述; 在确定 $phi, theta$ 后可以确定太阳光主方向 $arrow(L) = -(sin theta cos phi, sin theta cos phi, sin phi)^T$, 进而确定各镜的朝向和各项光学效率。计算镜面积 $A_i = a_i b_i$, 年平均功率
$
  W = 1/60 sum_("Date" in {0,30,...330} \
  "ST" in {9,10.5,12,13.5,15})
  sum_i "DNI" dot A_i dot eta_(cos,i) eta_(a,i) eta_("trunc",i) eta_(s,i) eta_(r,i) quad ("kW")
$
优化目标为镜单位面积的年平均功率 $f= W \/ sum_i A_i$; 考虑边界条件, 得到优化目标:

$
  & min_(x_i, y_i, h_i, a_i,b_i, x_c, y_c) f \
  & "s.t." cases(
      (x_i-x_j)^2 + (y_i-y_j)^2 >= a_i^2 quad forall i !=j,
      (x_i-x_c)^2 + (y_i - y_c)^2 >= R_(min)^2=100^2 quad forall i,
      x_c^2 + y_c^2 <= 350^2,
      x_i^2 + y_i^2 <= 350^2 quad forall i,
      a_i >= b_i quad forall i,
      h_i >= 1/2 b_i quad forall i,
      W >= 60000
    )
$

#SubSection("优化目标的简化")

#SubSub("参数范围限制")

#FLI() (1) 集热塔位置限制: 由于对称性, $x_c=0$; 由于该纬度位于北半球, 镜位于北侧更有利于余弦效率, $y_c<0$。

#FLI() (2) 最远距离限制: 初步实验下发现在给定范围内(半径350m的圆)年平均功率不会超过限制(60kW)太多, 故令 $R_(max)->infinity$ 舍弃对该参数的优化。

#SubSub("蒙特卡洛方法计算截断效率和阴影效率")

#FLI() 计算截断效率和阴影效率设计到对积分的近似, 故采用蒙特卡洛方法进行随机采样。包括2个部分: (1) 在镜面上随机选择点: 将镜面划分为4x4的网格, 在每一网格中随机选取一点; (2) 在光斑上随机选择点: 在光斑平面上围绕 $(0,0), 1/3 Omega |arrow(d)|(plus.minus 1,0), 1/3 Omega |arrow(d)|(plus.minus 1/2,plus.minus sqrt(3)/2)$ 7个中心中心随机选取7个点。

#SubSub("通过集热器参数生成镜的坐标")

#FLI() 镜分布的稀疏程度受镜间距和镜的尺寸共同决定, 因此在给定了镜间距 $D$ 和最远距离 $R_(max)$ 下可以通过一个最密排布流程得到所有镜的位置:

#h(40pt) a. $i=0$, 选取 $r_0 = R_(min)+1/2 D,Delta theta_0 = arcsin (D)/(2r_0)$。

#h(40pt) b. 围绕集热塔水平距离 $r_i$ 的区域内, 间隔 $2 Delta theta_i$ 添加新的镜的水平坐标, 坐标分布与镜场坐标系y轴对称, 到超出范围要求为止。第一个坐标与y轴夹角为 $0$ 或 $Delta theta_i$, 且与上一个环 $r_(i-1)$ 不同。

#h(40pt) c. 紧密排布下一个环 $r_(i+1) = r_i cos Delta theta_i + sqrt(D^2 - r_i^2 sin^2 Delta theta), Delta theta_(i+1) = Delta theta_i$。

#h(40pt) d. 查看是否满足 $r_(i+1) - r_i >= 1/2 D$, 若不满足, 则重新计算 $r_(i+1) = r_i + D, Delta theta_(i+1) = arcsin D/(2 r_(i+1))$。

#h(40pt) e. 重复步骤b至d直到环道第一个坐标位于要求区域外。

#SubSub("插值方法生成镜的参数")

#FLI() 为简化 $a_i, b_i,h_i$ 的优化, 我们可以假设这些参数随 $x_i,y_i$ 近似平滑变化, 并引入插值参数 $p_(a 0), ... p_(a 3), p_(b 0), ..., p_(b 3), p_(h 0), ... , p_(h 3) in [0,1]$, 插值得到
$
  a_i^* &= a(R_i, phi_i) = (p_(a 0) (R_i - R_1) /(R_0 - R_1) + p_(a 1) (R_i - R_0)/(R_1 - R_0) ) ( p_(a 2) (phi_i - phi_1)/(phi_0 - phi_1) + p_(a 3) (phi_i - phi_1)/(phi_1 - phi_0) )\
  b_i^* &= b(R_i,phi_i)=...\
  b_i^* &= b(R_i,phi_i)=...
$
其中 $(R_i,phi_i)$ 为 $(x_i,y_i)$ 的极坐标形式, 实际中令 $R_0=R_(min),R_1=R_(max),phi_0=0,phi_1=pi$。去归一化得到 $a_i = (D-5)a_i^*, b_i = a_i b_i^*, h_i = 2+h_i^* dot 4$。

#FLI() 最终优化目标被简化为

$
  & min_(D,y_c,p_(a 0),...,p_(h 3)) f \
  & "s.t." cases(
      2+5<D<8+5,
      -350<=y_c<=0,
      p_(a 0)\,...\,p_(h 3) in [0,1]
    )
$

#Section("优化算法")

#SubSection("粒子群算法优化单位镜面积功率")

#FLI() 粒子群算法是一种启发式的群体智能算法。对每一个个体 $arrow(p)_i$, 粒子群算法的更新方法为 $arrow(p)_i <- w dot arrow(p)_i + c_1 r_1 (arrow(p)^* - arrow(p)_i) + c_2 r_2 (arrow(p)_i^* - arrow(p)_i)$, 其中 $w$ 为惯性参数, $c_1, c_2$ 为学习参数, $r_1, r_2$ 为随机值, $arrow(p)^*$ 为群体的历史最佳位置, $arrow(p)_i^*$ 为该个体的历史最佳位置。本报告从零开始实现了针对定日场单位镜面积年均功率目标的粒子群算法。

#SubSection("惩罚函数法控制总功率")

#FLI() 最总功率要求, 设计惩罚函数 $g(W) = max(0, 60000-W)$, 实际优化目标变为 $min g(W) + f$; 考虑到粒子群算法依赖于历史最优值, 一个权重递增的惩罚函数优化虽然利于最优值的搜索, 但是会导致历史最佳值不可靠, 远离可行域, 为此对惩罚函数采用恒定权重。

#Section("结果展示和分析")

#FLI() 优化得到年均输出功率为 $60400 "kW"$, 单位镜面积年均输出功率 $0.639 "kW" / m^2$, 镜的长和宽均为 $4.76 "m"$, 集热塔底部位于定日场坐标系下的 $(0,-200.5,0) space ("m")$。

#FLI() 预期中镜的长和宽可能取得不同的值以减少阴影遮挡, 实际上二者均取得了在镜间距 $D$ 限制下的最大值($p_(a 0) = ... = p_(b 4) = 1$), 说明 $D$ 的限制已经充分分散了镜的分布; 集热塔位于南侧较远的地方, 此时北侧的镜能反射更多南侧的光照, 与预期相符。

#FLI() 各个镜的安装高度如@mirror_h 所示。

#[
  #set align(center)
  #figure(image("img/result_h.png"), caption: "镜安装高度和示意图")<mirror_h>
]

观察@mirror_h 可以发现, 镜的高度随到集热塔的距离和方位均有变化, 距离越远高度越高, 且南侧的镜最低, 北侧的镜最高。理论上外高内低有利于减少镜面间的遮挡, 提升阴影效率; 优化结果与预期相符。

#FLI() 各镜余弦效率如@eta_c 所示。

#[
  #set align(center)
  #figure(image("img/result_cos.png"), caption: "余弦效率示意图")<eta_c>
]

观察@eta_c 可以发现, 集热塔南北侧余弦效率差异明显, 说明余弦效率受光照影响大; 同时余弦效率随距离的变化并不明显。

#FLI() 各镜大气散射效率如@eta_a 所示。

#[
  #set align(center)
  #figure(image("img/result_atmos.png"), caption: "大气散射效率示意图")<eta_a>
]

观察@eta_a 可知, 散射效率随到集热塔的距离增加而降低, 而整体高于90%, 对总体效率低影响并不大。

#FLI() 各镜截断效率如@eta_t 所示。


#[
  #set align(center)
  #figure(image("img/result_trunc.png"), caption: "截断效率示意图")<eta_t>
]

观察@eta_t 可知, 在距离集热塔一定范围内, 截断效率接近100%, 在较远处, 截断效率随到集热塔的距离增加而迅速降低, 说明此时镜在集热塔投影灯光斑开始大于集热塔的接收范围, 开始造成较大的截断影响, 距离集热塔过近截断效率也有所降低, 这是因为集热部分在投影方向的面积较小。截断效率随位置呈现出较均匀的变化, 但存在一定的不规则变化。

#FLI() 各镜阴影效率如@eta_s 所示。

#[
  #set align(center)
  #figure(image("img/result_shad.png"), caption: "阴影效率示意图")<eta_s>
]

观察@eta_s 可知, 阴影效率随距离增加呈现处递减趋势。同时, 阴影效率随位置分布并不均匀: 在紧密排布策略造成的交界处, 阴影效率剧烈变化; 在特定方向上阴影效率普遍较低, 形成了 "暗条纹", 说明本实验使用的紧密排布策略存在优化空间。

#FLI() 综上可知, 余弦效率呈现出南北侧差异分布, 大气散射效率, 截断效率和阴影效率呈现出由近及远的差异分布, 本报告采取粒子群算法协调了各个因素的影响, 取得了较优的结果。

#Section("心得体会")

#FLI() 本次实验要求零依赖完成粒子群算法实现, 我们预计优化目标的计算量较大, 采用C++实现。事实也是如此, 大小为40的粒子群一次更新需要约350秒; 我们尝试使用cuda优化, 但并未成功, 在此过程中强化了编程能力。我们采取多次中断/恢复的方式持续执行程序, 每一次缩小可行域范围, 在2天时间内使结果趋于收敛。在此过程中, 我们发现粒子群部分个体被限制在可行域外的局部最优, 通过调试发现增大粒子群规模的效果最好; 我们也认识到了启发式算法存在的局限性。

#FLI() 在宗老师和戴老师的悉心指导下，本学期《最优化理论与方法》的课程学习圆满结束。这门课对于我们不仅是科研训练路径上的“工具箱”，更是一次从抽象理论到工程实践的深度思维重塑。

#FLI() 从线性规划的严谨逻辑，到无约束最优化的迭代演进，再到约束最优化中至关重要的最优性条件，课程每一个模块都环环相扣。宗老师和戴老师的讲解，让我们深刻理解了如何将复杂的现实约束转化为可求解的数学模型，这种对数学本质的洞察是后续所有算法应用的根基。

#FLI() 这门课最令我们受益匪浅的是其课堂实操，在“手把手教编程”的环节中，我们在课堂上亲自实现了从经典数值算法到启发式算法（如遗传算法、粒子群算法）的代码编写， 锻炼了我们的编程能力，拓宽了我们的视野，让我们学会处理非线性、非凸的复杂全局优化难题。 第八讲关于多学科优化设计与工程案例的分享，更是打破了“课本知识”与“工业实践”的墙壁，让我们看到了最优化技术在处理复杂工程系统时的巨大威力。

#Section("附录")

#SubSection("分工")

#FLI() 两名队员共同探讨选题，骆同学负责建模和编程，王同学负责撰写报告同时辅助骆同学。

#SubSection("代码说明")

#FLI() 程序目录组织如@file_layout 所示。

#[
  #set align(center)
  #set text(top-edge: 0.5em, bottom-edge: 0.5em)
  #set par(leading: 0.75em, spacing: 0.5em)
  #figure(
    table(
      columns: 2,
      table.hline(),
      table.hline(y: 1, stroke: 0.5pt),
      table.hline(y: 8),
      [文件], [说明],
      [CMakeLists.txt], [cmake工程文件],
      [coord_alg.cpp, coord_alg.hpp], [模型特定的算法和优化目标],
      [demo.py], [用于展示结果都python代码],
      [opt_alg.cpp, opt_alg.hpp], [粒子群优化算法],
      [vec_types.cpp, vec_types.hpp], [三维向量相关的运算功能],
      [test/test_dump_args.cpp],
      [将优化结果转换为可以表示的数据, 静态写入优化结果, 存入tmp.csv, 对应可执行文件test_dump.exe],
      [test/test_opt.cpp],
      [执行粒子群算法, 静态写入初始值范围信息和粒子群规模, 迭代步数等, 将每一步最优结果存入opt_track.csv, 对应可执行文件test_opt.exe],
    ),
    caption: "文件目录组织",
  )<file_layout>
]

#FLI() 复现代码需要cmake>=3.18和python, python的第三方库pandas(用于读取csv文件)和matplotlib(用于绘图)。按以下步骤使用代码:

#h(40pt) a. 在代码所在的文件夹下, 在命令行输入 `mkdir build && cd build && cmake ..` 实现cmake配置。

#h(40pt) b. 调整C++代码中有关粒子群规模和迭代次数等到参数, 并 `cmake --build .` 编译代码。

#h(40pt) c. `./test/test_opt` 执行粒子群优化, 观察命令行输出和opt_track.csv文件中的内容, 确定优化结果。

#h(40pt) d. 将优化结果以 `p = { ... };` 的形式写入test_dump_args.cpp中, `cmake --build .` 重新编译并执行 `./test/test_dump`, 得到tmp.csv。

#h(40pt) e. `python ./demo.py` 得到可视化结果。

#FLI() 逐一展示如下:

#[
  #set grid(stroke: 1pt, gutter: 0em, columns: 1fr, align: left, inset: 3pt)
  #set text(top-edge: 1em, bottom-edge: 0em)
  #set par(leading: 0.5em, spacing: 1em)
  #let tx1 = read("CMakeLists.txt")
  #let tx2 = read("coord_alg.hpp")
  #let tx3 = read("coord_alg.cpp")
  #let tx5 = read("opt_alg.cpp")
  #let tx6 = read("opt_alg.hpp")
  #let tx7 = read("vec_types.cpp")
  #let tx8 = read("vec_types.hpp")
  #let tx9 = read("test/test_dump_args.cpp")
  #let tx10 = read("test/test_opt.cpp")

  #grid(
    "CMakeLists.txt",
    raw(tx1, lang: "cmake"),
  )

  #grid(
    "coord_alg.hpp",
    raw(tx2, lang: "cpp"),
  )
  #grid(
    "coord_alg.cpp",
    raw(tx3, lang: "cpp"),
  )
  #grid(
    "opt_alg.hpp",
    raw(tx6, lang: "cpp"),
  )
  #grid(
    "opt_alg.cpp",
    raw(tx5, lang: "cpp"),
  )
  #grid(
    "vec_types.hpp",
    raw(tx8, lang: "cpp"),
  )
  #grid(
    "vec_types.cpp",
    raw(tx7, lang: "cpp"),
  )
  #grid(
    "test/test_dump_args.cpp",
    raw(tx9, lang: "cpp"),
  )
  #grid(
    "test/test_opt.cpp",
    raw(tx10, lang: "cpp"),
  )


]
