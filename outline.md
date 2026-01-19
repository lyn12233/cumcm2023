# nomenclature

- $D$ 间距
- $r_i$ 环 $i$ 的半径
- $\Delta\theta_i$ 环 $i$ 镜间夹角的一般
- $(x_i , y_i)$ 第 $i$ 个镜的坐标
- $(a_i , b_i)$ 第 $i$ 个镜的长宽
- $h_i$ 第 $i$ 个镜的高度
- $R_{min}, R_{max}$ 水平方向距塔最近, 最远距离
- $\Sigma = \{ (x,y) | {\chi}_{\Sigma} (x,y) \}$ 定日场区域
- $x_c , y_c$ 塔的坐标
- $h_{c2} , h_{c1}, r_c$ 塔的参数
- $\Omega$ 太阳角直径

# principles

- N个镜需要5N个参数,因此需要尽可能简化模型: (1) 在足够大和随机的场景下, $\eta_{sb}$ 应当只取决于 $a_i / D , b_i /D$, 因此可以先限制D的值; (2) 简化排布, $x_i , y_i$ 直接由 $R_{min} , R_{max} , D , x_c, y_c$ 生成; (3) 对 $a_i, b_i, h_i$ 根据 $(x_i , y_i)$ 和若干插值参数在极坐标下插值, 插值方法为 
  $$X (r, \phi) = ( p_0 \frac{ (r - r_1) (r - r_2) }{ (r_0 - r_1) (r_0 - r_2) } + p_1 \frac{ (r - r_0) (r - r_2) }{ (r_1 - r_0) (r_1 - r_2) } + p_2 \frac{ (r - r_0) (r - r_1) }{ (r_2 - r_0) (r_2 - r_1) } )\\
   ( p_3 \frac{ (\phi - \phi_1) (\phi - \phi_2) }{ (\phi_0 - \phi_1) (\phi_0 - \phi_2) } + ... )$$
    取 $r_0 = R_{min}, r_1 = \frac 1 2 (R_{min} + R_{max}), r_2 = R_{max}, \phi_0 = 0,\phi_1 = \pi/2, \phi_2 = \pi$, $p_1 ... p_5$ 为决策变量

- 镜的坐标系: 光照方向 $\vec L = -(\sin \theta \cos \phi , \cos \theta \cos \phi , \sin \phi)$ (?); 反射光方向 $\vec d  = (x_c, y_c, h_{c1} + \frac 1 2 h_{c2}) - (x_i, y_i, h_i)$ (记镜的位置$\mathrm{offs}_i$)对第 $i$ 个镜, (下省略下标), 镜的法向 $\vec N = norm ( norm (\vec d) - norm (\vec L) )$, 延伸方向 $\vec n_2 = norm (d_x, d_y, 0), \vec n_3 = \vec N \times \vec n_2$, 在镜的坐标系(标架 $\vec N, \vec n_2, \vec n_3$ )下, 镜的四个顶点 $(0, \pm \frac 1 2 b, \pm \frac 1 2 a)$, 世界坐标系到镜的坐标系的变换
    $$\vec x' = \begin{pmatrix}
        N_x & N_y & N_z\\
        n_{2x} & n_{2y} & n_{2z}\\
        n_{3x} & n_{3y} & n_{3z}\\
    \end{pmatrix} \vec x + \begin{pmatrix}
        \mathrm{offs} \cdot N\\
        \mathrm{offs} \cdot n_2\\
        \mathrm{offs} \cdot n_3\\
    \end{pmatrix}$$
- 镜和镜的遮挡: 假设在局部镜的方向一致, $j$ 镜对 $i$ 镜的遮挡: (1) $j$ 镜的中心 $p' = T ( (x_j, y_j, h_j) - (x_i, y_i,h_i) )$; (2) $p'_x > 0$ 可能投影; (3) 投影范围 $( 0, p'_y \pm \frac 1 2 b_j, p'_z \pm \frac 1 2 a_j )$ 为方形便于计算; (4) 将镜划为4x4网格用 `bool[4][4]` 描绘镜上的阴影, 网格中心的偏移 $(\Delta a_N, \Delta b_M) = (0, (\frac 1 4 N - \frac 3 8)a, (\frac 1 4 M - \frac 3 8)b)$
- 镜和塔的遮挡: 由于采样时刻固定, 塔的阴影位置相对固定, 考虑塔的阴影可能造成模型鲁棒性不佳;
- 截断效率: $S_c = 2r_c h_c \sqrt{ d_x^2 + d_y^2 } / |\vec d|, \space S_0 = |\vec d|^2 \sin^2 (\frac 1 2 \Omega), \space \eta_{trunc} = S_c / S_0$
- 镜的位置生成: (1) 镜占据的区域为半径 $\frac 1 2 D$ 的圆; (2) 镜围绕塔按照环排列, 环半径 $r_i$, 间距 $\Delta r_i = r_{i+1} - r_i$, 占据弧度 $2\Delta\theta_i$, 以2种方法生成:
    $$\begin{cases}
        & r_0 = R_{min} + \frac 1 2 D, \space \Delta\theta_0 = \arcsin \frac{D}{2r_0} & \\
        & r_{i+1} = r_i \cos \Delta \theta_i + \sqrt{ D^2 - r_i^2 \sin^2 \Delta \theta_i^2 }, \space \Delta \theta_{i+1} = \Delta \theta_i & \\
        &r_{i+1} = r_i + D, \space \Delta \theta_{i+1} = \arcsin \frac{D}{2 r_{i+1}} & (\Delta r_i < \frac 1 2 D)\\
    \end{cases}$$
    (3) $\theta_i^{[j]}$ 在y轴两边对称分布, 奇对称 $\{ (2N + 1) \Delta \theta_i \}$, 偶对称 $\{ 2N \Delta \theta_i \}$;
- 截断效率(2): 镜面过大, 指向能力减弱, 镜面各点偏移同时影响阴影效率和截断效率, 因此应逐点(网格)计算: 在反射光方向, 反射光发散为 $R=\sin \frac 1 2 \Omega \space |\vec d| + \frac 1 2 (\frac 1 4 a + \frac 1 4 b)$ 的圆, 圆的中心 $(x_{oN}, y_{oM}) = (\Delta a_N, \Delta b_M \eta_c)$, 接收部分为居中的长方形 $2r_c \times (h'=h_{c2} \sqrt{d_x^2 + d_y^2} / |\vec d|)$, 统计7个点 $(x_o, y_o), (x_o + 0.67 R, y_o), (x_o + 0.67\cdot 0.5 R, y_o + 0.67\cdot 0.866 R), ...$ 是否接收, 得到截断效率;