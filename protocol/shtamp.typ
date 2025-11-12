#let st(t) = text(size: 10pt, t)

#let mypage(it, stamp) = {
  page(
    table(
      columns: (auto, ) * 5 + (1fr, ) + (auto, ),
      rows: (1fr,) + (1.5em,) * 3,
      table.cell(colspan: 7, it),
      [], [], [], [], [], table.cell(rowspan: 3,text(size: 24pt,
      style: "italic", stamp)), [#st("Арк.")],
      [], [], [], [], [], table.cell(rowspan: 2, align(center + horizon, [#context counter(page).display()])),
      st[Змін.], st[Арк.], st[№ докум.], st[Підпис], st[Дата]
    ),
  )
}
