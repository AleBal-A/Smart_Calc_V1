FLAGS = -Wall -Wextra -Werror -g -std=c11 # -fsanitize=address
LFLAGS = -lcheck -lpthread -lm
SOURCES= calculate.c credit_calc.c help.c polish.c validation.c
OBJECTS= calculate.o credit_calc.o help.o polish.o validation.o
TARGET = Smart_Calculator

ifeq ($(shell uname -s), Linux)
        LFLAGS = -lm -lpthread -lcheck -lsubunit
endif

all: install

# Установка
install:
	@clear
	@echo "Installation of S21_SmartCalc_v1.0:"
	@tput setaf 2; echo "███████╗███╗   ███╗ █████╗ ██████╗ ████████╗     ██████╗ █████╗ ██╗      ██████╗     ██╗    ██████╗ "; tput sgr0
	@tput setaf 2; echo "██╔════╝████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝    ██╔════╝██╔══██╗██║     ██╔════╝    ███║   ██╔═████╗"; tput sgr0
	@tput setaf 2; echo "███████╗██╔████╔██║███████║██████╔╝   ██║       ██║     ███████║██║     ██║         ╚██║   ██║██╔██║"; tput sgr0
	@tput setaf 2; echo "╚════██║██║╚██╔╝██║██╔══██║██╔══██╗   ██║       ██║     ██╔══██║██║     ██║          ██║   ████╔╝██║"; tput sgr0
	@tput setaf 2; echo "███████║██║ ╚═╝ ██║██║  ██║██║  ██║   ██║       ╚██████╗██║  ██║███████╗╚██████╗     ██║██╗╚██████╔╝"; tput sgr0
	@tput setaf 2; echo "╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝        ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝     ╚═╝╚═╝ ╚═════╝ "; tput sgr0
	@mkdir -p ../build
	@cd ./Smart_Calc_QT && qmake && make && make clean && rm -rf Makefile && mv Smart_Calc_QT.app ../../build/$(TARGET).app
	@clear
	@tput setaf 2; echo "██████╗  ██████╗ ███╗   ██╗███████╗"; tput sgr0
	@tput setaf 2; echo "██╔══██╗██╔═══██╗████╗  ██║██╔════╝"; tput sgr0
	@tput setaf 2; echo "██║  ██║██║   ██║██╔██╗ ██║█████╗  "; tput sgr0
	@tput setaf 2; echo "██║  ██║██║   ██║██║╚██╗██║██╔══╝  "; tput sgr0
	@tput setaf 2; echo "██████╔╝╚██████╔╝██║ ╚████║███████╗"; tput sgr0
	@tput setaf 2; echo "╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚══════╝"; tput sgr0
	@echo "Installation completed! You can find app in the ../build/ folder!"
 
uninstall:
	@rm -rf ../build
	@echo "Uninstall completed!"

# для сборки всех .c файлов в исполняемый
test:
	gcc $(FLAGS) $(SOURCES) -c test.c
	gcc $(FLAGS) test.o $(OBJECTS) $(LFLAGS) -o test.out
	rm -rf *.o 
	clear
	./test.out

# Создание документации
dvi:
	doxygen dvi_config
	open ./html/index.html

# Архиваация
dist: install
	mkdir -p ../dist
	mkdir -p ../dist/src
	cp -R ../build/$(TARGET).app ../dist
	cp -R . ../dist/src
	cp -R ./Smart_Calc_QT ../dist/src
	cd ../dist && tar cvzf $(TARGET).tgz *
	cd ../dist && rm -rf $(TARGET).app
	rm -rf ../dist/src
	echo "Archive creation completed successfully!"

# Покрытие
gcov_report: clean add_coverage test
	./test.out
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	rm -rf *.gcda *.gcno test.info
	open report/index.html

add_coverage:
	$(eval FLAGS += --coverage)

checks:
	clang-format -n *.c *.h
	clang-format -style="{BasedOnStyle: Google}" -i *.c Smart_Calc_QT/*.cpp 
	clang-format -style="{BasedOnStyle: Google}" -i *.h Smart_Calc_QT/*.h
	cppcheck enable=all missingIncludeSystem *.c

clean:
	rm -rf *.o *.out gcov_report *.a *.gcda *.gcno test test.info s21_string.a a.out.dSYM ./report

# Gitlab ========
add:
	git add Makefile s21_*.c s21_matrix.h .clang-format

.PHONY: commit

commit:
	@echo "Введите сообщение для коммита:"
	@read commit_message; \
	git add Makefile .clang-format *.c *.h; \
	git commit -m "$$commit_message"; \
	git push origin develop
# ===============