# lab05
 Лабораторная работа №5
 
 1)Клонирую репозиторий с заданием
  
 git clone
 
 cd lab05 
 
 Отвязываю его 
 
 git remote remove
 
 Привязываю к заранее созданному 
 
 git add remote
 
 2)Для удобства удаляю картинку и задание в README.md
 
 3)cd banking - заполняю CMakeLists.txt для banking 
 
 cmake_minimum_required(VERSION 3.4)
 
project(bank_lib)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(banking STATIC Account.cpp Account.h Transaction.cpp Transaction.h)

cd ..

4)Скачиваю gtest git submodule add https://github.com/google/googletest.git

5)Создаю папку с тестами, там код

#include <Account.h>
#include <gtest/gtest.h>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>

TEST(Account, Banking){
//создаём тестовый объект
	Account test(0,0);
//проверяем GetBalance (и заодно конструктор)
	ASSERT_EQ(test.GetBalance(), 0);
//проверяем, что фича lock работает нормально
	ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
	test.Lock();
	ASSERT_NO_THROW(test.ChangeBalance(100));
//проверяем, что изменение баланса работает
	ASSERT_EQ(test.GetBalance(), 100);
//проверяем, что залочить уже залоченное нельзя
	ASSERT_THROW(test.Lock(), std::runtime_error);
//проверяем, что анлок работает
	test.Unlock();
	ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
//радуемся жизни
}

TEST(Transaction, Banking){
//создаём константы. base_fee для полноценного теста должен быть хотя бы 51.
	const int base_A = 5000, base_B = 5000, base_fee = 100;
//создаём тестовые объекты
	Account Alice(0,base_A), Bob(1,base_B);
	Transaction test_tran;
//проверяем конструктор по умолчанию и сеттеры-геттеры
	ASSERT_EQ(test_tran.fee(), 1);
	test_tran.set_fee(base_fee);
	ASSERT_EQ(test_tran.fee(), base_fee);
//проверяем случаи когда транзакция не проходит
	ASSERT_THROW(test_tran.Make(Alice, Alice, 1000), std::logic_error);
	ASSERT_THROW(test_tran.Make(Alice, Bob, -50), std::invalid_argument);
	ASSERT_THROW(test_tran.Make(Alice, Bob, 50), std::logic_error);
	if (test_tran.fee()*2-1 >= 100)
		ASSERT_EQ(test_tran.Make(Alice, Bob, test_tran.fee()*2-1), false);
//проверяем, что всё правильно лочится
	Alice.Lock();
	ASSERT_THROW(test_tran.Make(Alice, Bob, 1000), std::runtime_error);
	Alice.Unlock();
//проверяем что если входные параметры правильные, то транзакция проходит православно
	ASSERT_EQ(test_tran.Make(Alice, Bob, 1000), true);
	ASSERT_EQ(Bob.GetBalance(), base_B+1000);	
	ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
//проверяем что транзакция не проходит, если не хватает средств
	ASSERT_EQ(test_tran.Make(Alice, Bob, 3900), false);
	ASSERT_EQ(Bob.GetBalance(), base_B+1000);	
	ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
}

6)Пишу общий CMakeLists.txt в lab05

cmake_minimum_required(VERSION 3.4)

SET(COVERAGE OFF CACHE BOOL "Coverage")

SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(lab)

#set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gtest" "gtest")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp) // местоположение моих тестов

if (COVERAGE)  // если определена переменная, то добавляются опции библиотеки и компилятора

    target_compile_options(tests PRIVATE --coverage)

    target_link_libraries(tests PRIVATE --coverage)

endif()

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/banking)

target_link_libraries(tests PRIVATE gtest gtest_main gmock_main banking)

7)Создаю CI.yml в .github/workflows/

name: Banking

on:
  push:
    branches: 
    - master

jobs:
  
  Build:
    
    runs-on: ubuntu-latest

    steps:
  
    - uses: actions/checkout@v2

    - 
    name: Build banking
      
      run: |
      
        cd banking
      
        cmake -H. -B_build
      
        cmake --build _build
  
  
  Test
  ing:
    ru
    ns-on: ubuntu-latest
    
    
    steps:
    - 
    uses: actions/checkout@v2
    - 
    name: Update
      
      run: |
      
        sudo apt install git && git submodule update --init
      
        sudo apt install lcov
      
        sudo apt install g++
      
      
    - 
    name: Testing
      
      run: |
   
        mkdir _build
   
        cd _build
   
        CXX=/usr/bin/g++ cmake -DCOVERAGE=1 ..
   
        cmake --build .
   
        ./tests
   
        lcov -t "banking" -o lcov.info -c -d .
        
    - name: Coveralls
   
      uses: coverallsapp/github-action@master
   
    
      with:
   
        github-token: ${{ secrets.github_token }}
   
        parallel: true
      
        path-to-lcov: ./_build/lcov.info
      
        coveralls-endpoint: https://coveralls.io
        
    - name: Coveralls Finish
      
      uses: coverallsapp/github-action@master
      
      with:

        github-token: ${{ secrets.github_token }}

        parallel-finished: true
        
  8)nano .gitmodules 
  
  [submodule "gtest"]

path = gtest

url = https://github.com/google/googletest.git

- указываю URL для подмодуля.

9) Проверяю покрытие.
