#include "mysort.h"

int main()
{
    std::vector<int> vec{2, 5, 8, 4, 9, 6};
    mysort ms;
    // ms.bubble_sort(vec);
    ms.choose_sort(vec);
    // ms.insert_sort(vec);
    //  ms.shell_sort(vec);
    // ms.buck_sort(vec);
    // ms.merge_up2down(vec, 0, vec.size() - 1);
    //--------------------------------------------------------------------------------------
    // 堆排，快排(nlogn)，基排(在计数之上建立，把一个数字分成很多位，每一位都进行排序，从高到低)

    // ms.cout_sort(vec);
    // ms.heap_sort(vec);
    // ms.fast_sort(vec, 0, vec.size() - 1);
    
    ms.print_num(vec);
}