#set page(
  margin: (top: 2cm, bottom: 2cm, left: 3cm, right: 1cm), 
)
#set text(lang: "UK",   font: "Times New Roman",
  size: 14pt)
#set par(leading: 1.5em, )
//title
#align(center)[
  НАЦІОНАЛЬНИЙ ТЕХНІЧНИЙ УНІВЕРСИТЕТ УКРАЇНИ

«КИЇВСЬКИЙ ПОЛІТЕХНІЧНИЙ ІНСТИТУТ 

імені ІГОРЯ СІКОРСЬКОГО»

Кафедра системного програмування і 

спецалізованих комп'ютерних систем

\
#text(16pt)[*КУРСОВИЙ ПРОЄКТ*]

з дисципліни «Комп’ютерні мережі»

на тему: Маршрутизація в мережі передачі даних
#v(1.5em * 2)
#align(right)[
  Студента #underline("IV") курсу, групи #underline("КВ-22") 
  #block(width: auto)[#align(left)[за спеціальністю

  123 «Комп'ютерна Інженерія»

  #underline[Ковкіна В.В.]]] #v(1.5em)
  Керівник: доцент, к.т.н., доц. Орлова М.М. #v(1.5em)
  Національна оцінка #underline(" " * 27) #v(1.5em)
  Кількість балів: #underline(" " * 4) Оцінка: ECTS #underline(" " * 4)  
  #v(1.5em * 2)
  Члени комісії #underline(" " * 10) доцент, к.т.н. Орлова М.М. #linebreak()
  #underline(" " * 12) доцент, к.т.н Щербина О.А
]
#place(bottom + center, "Київ - 2025")
]
#pagebreak() 
//
#set align(center)
*НАЦІОНАЛЬНИЙ ТЕХНІЧНИЙ УНІВЕРСИТЕТ УКРАЇНИ*

*КИЇВСЬКИЙ ПОЛІТЕХНІЧНИЙ ІНСТИТУТ* 

*імені ІГОРЯ СІКОРСЬКОГО»*
#v(1.5em)
ФАКУЛЬТЕТ ПРИКЛАДНОЇ МАТЕМАТИКИ
#v(1.5em)
Кафедра системного програмування і спеціалізованих комп'ютерних систем
#v(1.5em)
#set align(left)
Рівень вищої освіти – #underline("перший (бакалаврський)")

Спеціальність  #underline("123  «Комп’ютерна інженерія»")

\
\
\
#set align(center)
*ЗАВДАННЯ*

*на курсовий проєкт студента*

#underline("Ковкіна Владислава")
#set align(left)
1. Тема проєкту:#underline(" Маршрутизація в мережі передачі даних")
керівник проєкту: #underline[к.т.н., доцент Мартинова Оксана Петрівна]
 2. Термін подання студентом проєкту:  #underline( "грудень 2025 р.")
 3. Вихідні дані до проєкту: #underline()[Структура мережі –  24 комунікаційних вузлів, 2  
 канали – супутникові. Середній ступінь мережі – 4. Ваги каналів - 1, 2, 4, 5, 6, 7, 8, 10, 15, 21. Алгоритм --- маршрутицації з урахуванням стану каналів зв'язку)]

 4. Зміст пояснювальної записки: #underline()[Вступ. Основні теоретичні відомості. Опис 
заданого алгоритму маршрутизації. Основна частина. Опис процесу тестування 
передачі повідомлень. Аналіз та порівняння отриманих результатів. Висновки. 
Список використаної літератури] 

#pagebreak()
#set align(center)
Календарний план
#let rows = (
  ("Отримання теми та завдання", "2"),
  ("Підбір та вивчення літератури", "3 – 5"),
  ("Виконання завдання 1", "6 – 7"),
  ("Виконання завдання 2", "8 – 9"),
  ("Виконання завдання 3", "10 – 12"),
  ("Виконання завдання 4", "13 – 14"),
  ("Виконання завдання 5", ""),
  ("Подання курсового проєкту на перевірку", "15"),
  ("Захист курсового проєкту", "16"),
)
#show table: set par(leading: 1em)

#table(
  columns: (auto, 3fr, auto, auto),
  align: (left, ) * 2 + (auto, ) * 2,
  stroke: 0.5pt,
  
  inset: 6pt,
  [№ з/п], [Назва етапів виконання курсового проєкту], [Тиждень семестру], [Примітка],
  ..for(i, row) in rows.enumerate(){
    ([#(i + 1).], [#row.at(0)], [#row.at(1)], [Виконано])
  },
)
\
\
#set align(horizon)
#table(
  columns: (1fr, 1fr, 1fr),
  align: (left, center, right),
  inset: 0pt,
  stroke: none,
  row-gutter: 0.5em,

  [Студент], [#underline(" ")] * 40, [*Ковкін Владислав*],

  [], [(підпис)], [(ім’я, прізвище)],
  [#v(1.5em)],[#v(1.5em)],[#v(1.5em)],

  [Керівник проєкту], [#underline(" ")] * 40, [*Оксана Мартинова*],

  [], [(підпис)], [(ім’я, прізвище)],
)
#pagebreak()
#set align(center + top)
#set par(first-line-indent: 1.25cm)
АНОТАЦІЯ\
#set align(left)
Курсовий проект включає пояснювальну записку (40 с., 28 рис., 5 табл.). 
В даному курсовому проєкті було розроблено програму для симуляції 
процесу передачі повідомлень в мережі передачі даних. Основна мета програми 
полягає у моделюванні процесу вибору маршруту для передачі даних у мережі з 
певною конфігурацією. Програма може аналізувати час передачі повідомлень, 
кількість інформаційних та службових пакетів.\

Для зручності роботи з програмою, був розроблений візуальний 
інтерфейс, що дає можливість налаштовувати параметри мережі передачі даних. 
У проєкті також є теоретична частина, де розглянуті основні теоретичні поняття 
та надано інформацію, що стосується визначеної мережі відповідно до 
завдання. 

Під час розробки програми, були проведені тести з різними наборами 
даних, а також досліджені взаємозв'язки між часом передачі і обсягом 
службової інформації залежно від розміру пакету та повідомлення. 
Програма була реалізована на мові програмування C++.

*Ключові слова*: комп’ютерні мережі; передача даних; маршрутизація; 
користувач; вузли; логічне з’єднання; віртуальний канал; дейтаграмний режим; 
аналіз результатів.

#pagebreak()
#set align(center + top)
#set par(first-line-indent: 1.25cm)
ABSTRACT\
#set align(left)
The course project includes an explanatory note (40 pages, 28 figures, 5 tables).  
In this course project, a program was developed to simulate the process of message 
transmission in a data communication network. The main purpose of the program 
is to model the process of selecting a route for data transmission within a network 
of a given configuration. The program can analyze the transmission time of messages, 
the number of data and service packets.

For convenient interaction with the program, a graphical user interface was developed, 
allowing users to configure parameters of the data transmission network.  
The project also contains a theoretical part that presents the fundamental concepts 
and provides information related to the specific network defined by the assignment.

During the development process, tests were conducted with various data sets, 
and relationships were studied between transmission time and the amount of 
service information depending on the size of packets and messages.  
The program was implemented in the C++ programming language.

*Keywords*: computer networks; data transmission; routing; user; nodes; logical connection; 
virtual channel; datagram mode; result analysis.

#pagebreak()
#label("outline-start")
#let outl = [#align(center)[ЗМІСТ]]
#import "shtamp.typ" : *

#mypage(outl, "ІАЛЦ. 467200.002 ТЗ")
