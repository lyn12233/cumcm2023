
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

#Section("数学建模")

#SubSection("符号约定")

#SubSection("模型的建立")

#SubSection("优化目标的简化")

#SubSection("蒙特卡洛方法计算截断效率和阴影效率")

#Section("优化算法")

#SubSection("粒子群算法优化单位镜面积功率")
#SubSection("惩罚函数法控制总功率")

