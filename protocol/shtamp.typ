
#let mypage(it, stamp) = {
  page(
    table(
      columns: (auto, ) * 5 + (1fr, ) + (auto, ),
      rows: (1fr,) + (1.5em,) * 3,
      table.cell(colspan: 7, it),
      [], [], [], [], [], table.cell(rowspan: 3, stamp), "Арк.",
      [], [], [], [], [], table.cell(rowspan: 2, [#context counter(page).display()]),
      [Змін.], [Арк.], [№ докум.], [Підпис], [Дата]
    ),
  )
}

#mypage("dlf", "dkjf")

