/********************************************************************************* 
  * FileName: Timestamp_test.cpp
  * Date 2020/4/3
  * Description: 
**********************************************************************************/
#include "base/Timestamp.h"
#include <vector>
#include <stdio.h>

using namespace annotate_muduo;

void passByConstReference(const Timestamp& x){
    printf("%s\n",x.toString.c_str());
}


