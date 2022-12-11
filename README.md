# Алгоритм Эрли

Программа принимает файл с исходной грамматикой и отвечает на запросы о принадлежности слова грамматике.

### Сборка проекта:
1. Скачать репозиторий, перейти на ветку earley
2. Из папки репозитория в терминале выполнить _mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && make tests_
3. Для запуска тестов выполнить _../bin/Test_
4. Для запуска обработки из текстового файла выполнить _../bin/Main_

### Формат входных данных для текстового файла
Данные в файле находятся по строкам в следующем порядке:
1. Нетерминальные символы(через пробел или слитно)
2. Терминальные символы(через пробел или слитно)
3. Символ, разделяющий левую и правую часть правила грамматики
4. ε-символ
5. Стартовый символ

Примечание: среди символов не должен использоваться #.

Далее по одному в строке вводятся правила грамматики вида <левая часть правила><разделяющий символ><правая часть правила>.
Для того, чтобы считать файл, необходимо поместить его в каталог репозитория "grammar" и ввести его название в терминал после запуска программы.
После этого можете построчно вводить слова, принадлежность к грамматике которых Вы хотите проверить. Для завершения работы введите _!exit!_ в терминал.

_Пример файла с входными данными:_

> S\
> ( )\
> ^\
> e\
> S\
> S^e\
> S^(S)S

Так же примеры входных файлов находятся в каталоге grammar репозитория.

_Покрытие строк тестами можно посмотреть в каталоге build/coverage/ в файле index.html._

### UML-диаграмма алгоритма:

```mermaid
   classDiagram
   Earley*--Situation : 0..*
   Earley*--Grammar : 1
   Grammar: + Grammar()
   Grammar: + Grammar(string, string, char, char, char, vector<string>)
   Grammar: # SetNonTerminals(string)
   Grammar: # SetTerminals(string)
   Grammar: # AddRule(string)
   Grammar: # SetSeparator(char)
   Grammar: # SetEpsilon(char)
   Grammar: # SetStartSymbol(char)
   Earley: - Scan(size_t, char)
   Earley: - Complete(size_t)
   Earley: - Predict(size_t)
   Earley: - EarleyAlgorithm(string)
   Earley: + IsBelongToGrammar(string)
   Earley: - vector situation_layers_
   Grammar: # map grammar_rules_
   Grammar: # unordered_set non_terminals_
   Grammar: # unordered_set terminals_
   Grammar: # char separator_
   Grammar: # char epsilon_
   Grammar: # char start_
   Situation: + char start
   Situation: + string rule
   Situation: + size_t point_position
   Situation: + size_t layer_position
   Situation: + size_t len
   Situation: + Situation()
   Situation: + Situation(char, string, size_t, size_t, char)
```