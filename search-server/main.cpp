// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:

// Закомитьте изменения и отправьте их в свой репозиторий.


#include <iostream>

using namespace std;

int main(){
    int threes_count = 19 * 9 + 10;

    /*for(int i = 13; i < 1000; ++i) {
           // cout << "entered 'for' "s << endl;
        if(i / 100 == 3) {
            ++threes_count;
            cout << "one more '3' in //100 "s << endl;
            continue;
        }
        if(i > 100) {
            i = i % 100;
        }
        if(i / 10 == 3 || ((i % 10) % 3 == 0)) {
            ++threes_count;
            cout << "one more '3' "s << endl;
        }
    }*/
    cout << threes_count <<endl;
    return 0;
}