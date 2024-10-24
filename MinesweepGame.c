//#include <conio.h>   // 引入控制台输入输出库，用于读取与显示文本(Windows,MS-DOS)
#include <stdio.h> // 引入标准输入输出库，用于打印和读取数据(LINUX,UNIX)
#include <stdlib.h>  // 引入标准库，用于动态内存分配和随机数生成  
#include <time.h>    // 引入时间库，用于处理时间和随机数种子  

#define LEN 20 // 定义常量LEN为20，表示网格的宽度（或长度）  
#define MSG(x) MsgLst[x].Msg // 定义宏MSG(x)，用于获取消息数组中的字符串消息  
#define VAR(x) vararr[x] // 定义宏VAR(x)，用于获取vararr数组中的变量值  

// 定义一个枚举类型，用于表示一些常量  
enum {  
    I,          // 循环索引I  
    J,          // 循环索引J  
    K,          // 用户的选择索引K  
    LENGTH,     // 网格的有效长度（行数）  
    DIFF,       // 难度值  
    SAFECOUNT,  // 安全格子计数  
    WALL,       // 墙的标识，表示一种特殊类型的格子  
    MINE,       // 地雷的标识，表示地雷类型的格子  
    SAFE,       // 安全格子的标识  
    SELECTED,   // 已选择的格子的状态  
    UNSELECTED, // 未选择的格子的状态  
    MARK        // 标记状态，可能用于标记地雷或安全格子  
};  

// 定义一个Cell结构体，用于表示每个格子的状态  
typedef struct {  
    short type;    // 该格子的类型（例如：地雷、安全、墙等）  
    short status;  // 该格子的状态（例如：选中、未选中、标记等）  
    short remain;  // 该格子周围的地雷数量  
} Cell; // Cell结构体定义完成  

// 定义一个Msg结构体，用于存储消息字符串  
typedef struct {  
    char *Msg; // 消息的内容，最多60个字符  
} Msg; // Msg结构体定义完成  

// 自定义函数：memset，用于初始化数组  
short memsetA(short* arr, int size, int value) {  
    short vararr[2] = { 1,0 }; // 创建一个临时数组，用于内部计数和返回值  
    for (VAR(1) = 0; VAR(1) < size; VAR(1)++) { // 循环，遍历数组  
        arr[VAR(1)] = value; // 将数组的每个元素都设置为指定的value  
    }  
    VAR(0) = 0; // 设置返回值为0，表示成功  
    return VAR(0); // 返回状态，0表示成功  
}  

// 自定义函数：MineAssign，根据盐值确定Cell的类型（地雷或安全）  
Cell MineAssign(int salt) {  
    Cell res; // 创建一个Cell结构体的实例  

    // 根据随机数和盐值决定该Cell是地雷还是安全  
    res.type = (rand() % 10 < salt) ? MINE : SAFE; // 如果随机数小于盐值，则为地雷；否则为安全  
    res.status = UNSELECTED; // 初始状态为未选择  
    res.remain = -1; // 初始的剩余地雷数设置为-1，表示未知  

    return res; // 返回生成的Cell  
}  

// 自定义函数：generateWall，生成墙的Cell  
Cell generateWall() {  
    Cell res; // 创建一个Cell结构体的实例  
    res.type = WALL;
    res.status = WALL;
    res.remain = -1; // 设置类型和状态为墙，剩余地雷数为-1  

    return res; // 返回生成的墙的Cell  
}  

// 自定义函数：lim，用于限制输入在给定的范围内  
short lim(int input, int min, int max) {  
    // 如果input在min和max之间，返回input；否则返回最小值或最大值  
    return ((input >= min) && (input <= max)) ? (input) : ((input < min) ? min : max);  
}  

// 自定义函数：MineCount，计算某个Cell周围的地雷数量  
short MineCount(Cell* Minefield, short row, short col) {  
    short res = 0; // 初始化地雷计数为0  
    // 检查周围8个方向的格子，如果是地雷，计数增加  
    //当选择的Cell不在最右侧
    if (col != (LEN - 1)) {
        if (Minefield[row * LEN + col + 1].type == MINE) res++; // 右
        if (row && Minefield[(row - 1) * LEN + col + 1].type == MINE) res++; // 右上
        if (row != (LEN - 1) && Minefield[(row + 1) * LEN + col + 1].type == MINE) res++; // 右下
    }
    //当选择的Cell不在最左侧
    if (col) {
        if (Minefield[row * LEN + col - 1].type == MINE) res++; // 左  
        if (row != (LEN - 1) && Minefield[(row + 1) * LEN + col - 1].type == MINE) res++; // 左下  
        if (row && Minefield[(row - 1) * LEN + col - 1].type == MINE) res++; // 左上 
    }   
    if (row && Minefield[(row - 1) * LEN + col].type == MINE) res++; // 上  
    if (row != (LEN - 1) && Minefield[(row + 1) * LEN + col].type == MINE) res++; // 下  
      
    return res; // 返回周围地雷的数量  
}

int main(){
    short* vararr; // 声明一个短整型指针，用于动态分配和存储变量数组  
    Cell* CellField; // 声明一个Cell指针，用于动态分配和存储地雷场的格子  
    Msg* MsgLst; // 声明一个Msg指针，用于动态分配和存储消息列表  

    srand((unsigned)time(0)); // 初始化随机数生成器，以当前时间为种子  

    // 动态分配内存  
    vararr = (short*)malloc(sizeof(short) * 6); // 为 vararr 分配空间，用于存储6个短整型值  
    CellField = (Cell*)malloc(sizeof(Cell) * LEN * LEN); // 为 CellField 分配空间，用于 LEN x LEN 的格子存储  
    MsgLst = (Msg*)malloc(sizeof(Msg) * 16); // 为 MsgLst 分配空间，用于存储16个消息  

    // 初始化消息列表  
    MSG(0) = "Mine Sweep Game Ver1.0"; // 游戏版本信息  
    MSG(1) = "Author:: Echo 6-4"; // 作者信息  
    MSG(2) = "Enter Minefield size(9~20):: "; // 提示输入地雷场大小  
    MSG(3) = "Enter Difficulty(2~8):: "; // 提示输入游戏难度  
    MSG(4) = "00 01 02 03 04 05 06 07 08 09 10 "; // 列出列索引  
    MSG(5) = "11 12 13 14 15 16 17 18 19 20 \n"; // 列出列索引  
    MSG(6) = "Enter the Row index of minefield:: "; // 提示输入行索引  
    MSG(7) = "Enter the Col index of minefield:: "; // 提示输入列索引  
    MSG(8) = "It's a Safe Cell. There are "; // 提示用户安全格子及周围地雷数量  
    MSG(9) = " Mine Cells nearby \n"; // 提示信息的后半部分，指明附近地雷数量  
    MSG(10) = "You triggered a Mine!! Game Over !!"; // 地雷触发，游戏结束提示  
    MSG(11) = "You have clear the MineField Safely! Good Work!!!"; // 游戏成功完成提示  
    MSG(12) = "Generating MineField.... \n"; // 生成地雷场的提示信息  
    MSG(13) = "Enter your choice(1->mark,2->select,3->rechoose)::"; // 提示用户选择操作  
    MSG(14) = "You can't do anything to a wall.\n"; // 对墙的操作提示  
    MSG(15) = "Incorrect Input!!\n"; // 输入错误提示  

    // 初始化vararr数组  
    memsetA(vararr, 6, 0); // 调用之前定义的memset函数，初始化vararr的前6个元素为0  

    // 显示游戏版本和作者信息  
    printf("%s\n%s\n\n", MSG(0), MSG(1)); // 打印游戏版本和作者信息  

    // 获取地雷场的尺寸  
    printf("%s", MSG(2)); // 提示用户输入地雷场尺寸  
    scanf("%hd", &VAR(I)); // 从标准输入读取行输入并存储在VAR(I)中  
    fflush(stdin); // 清空输入缓冲区（通常不推荐使用，取决于编译器）  

    // 限制输入尺寸在9到20之间  
    VAR(LENGTH) = lim(VAR(I), 9, LEN); // 调用lim函数确保输入值在范围内  
    VAR(I) = 0; // 将VAR(I)重置为0  

    // 获取难度级别  
    printf("%s", MSG(3)); // 提示用户输入难度  
    scanf("%hd", &VAR(I)); // 读取标准输入中的难度值  
    fflush(stdin); // 清空输入缓冲区  

    // 限制难度输入在2到8之间  
    VAR(DIFF) = lim(VAR(I), 2, 8); // 调用lim函数确保输入值在范围内  
    VAR(I) = 0; // 将VAR(I)重置为0  

    // 显示生成地雷场的提示  
    printf("%s", MSG(12));  

    // 打印列索引的提示，分为两行显示  
    printf("%s %s", MSG(4), MSG(5));  
	
    VAR(SAFECOUNT)=0;
    // 生成地雷场，使用循环遍历每个格子  
    for (VAR(I) = 1; VAR(I) <= LEN * LEN; VAR(I)++) {  
        // 每当到达新行时，增加行计数并打印行号  
        if (!((VAR(I) - 1) % LEN)) {  
            VAR(J)++;  
            printf((VAR(J) >= 10) ? ("%d ") : ("0%d "), VAR(J)); // 行号格式化  
        }  
    
        // 根据当前索引填充地雷场  
        if (((VAR(I) - 1) < (LEN * VAR(LENGTH))) && (((VAR(I) - 1) % LEN) < VAR(LENGTH))) {  
            // 在地雷区域随机放置地雷或安全格  
            CellField[VAR(I) - 1] = MineAssign(VAR(DIFF));  
            if (CellField[VAR(I) - 1].type == SAFE) {  
                VAR(SAFECOUNT)++; // 安全格子计数  
            }  
            printf("*  "); // 显示安全格的标志  
        } else {  
            // 生成墙壁  
            CellField[VAR(I) - 1] = generateWall(); // 生成墙  
            printf("W  "); // 显示墙壁的标志  
        }  
    
        // 每当完成一行后换行  
        if (!(VAR(I) % LEN))  
            printf("\n");  
    }  

    // 重置长度和难度变量  
    VAR(LENGTH) = 0;   
    VAR(DIFF) = 0;

// 输入处理开始  
_input:  
    // 提示用户输入行索引  
    printf("%s", MSG(6));  
    scanf("%hd", &VAR(DIFF)); // 读取行索引  
    fflush(stdin); // 清空输入缓冲区
    VAR(DIFF)--; // 将行号减一以兼容计算机排序

    // 提示用户输入列索引  
    printf("%s", MSG(7));  
    scanf("%hd", &VAR(LENGTH)); // 读取列索引  
    fflush(stdin); // 清空输入缓冲区  
    VAR(LENGTH)--;// 将列号减一以兼容计算机排序

    // 提示用户输入操作选择  
    printf("%s", MSG(13));  
    scanf("%hd", &VAR(K)); // 读取用户选择的操作  
    fflush(stdin); // 清空输入缓冲区  

    // 根据用户选择的操作进行不同处理  
    switch (VAR(K)){  
        case 1: // 标记操作  
            // 检查当前单元格是否为墙  
            if (CellField[VAR(DIFF) * LEN + VAR(LENGTH)].type != WALL) {  
                // 将单元格标记为已标记状态  
                CellField[VAR(DIFF) * LEN + VAR(LENGTH)].status = MARK;  
            } else {  
                // 如果是墙，提示用户  
                printf("%s", MSG(14));  
                goto _input; // 回到输入处理  
            }  
            break;  
        case 2: // 选择操作  
            // 如果选择的单元格是地雷  
            if (CellField[VAR(DIFF) * LEN + VAR(LENGTH)].type == MINE) {  
                printf("%s", MSG(10)); // 告知用户触发地雷  
                goto _end; // 跳转到结束处理  
            }  
            // 如果选择的单元格是安全的  
            if (CellField[VAR(DIFF) * LEN + VAR(LENGTH)].type == SAFE) {  
                VAR(SAFECOUNT)--; // 安全格子计数减一  
                CellField[VAR(DIFF) * LEN + VAR(LENGTH)].status = SELECTED; // 更新状态为已选择  
                CellField[VAR(DIFF) * LEN + VAR(LENGTH)].remain = MineCount(CellField, VAR(DIFF), VAR(LENGTH)); // 计算附近地雷数  
                printf("%s %d %s", MSG(8), CellField[VAR(DIFF) * LEN + VAR(LENGTH)].remain, MSG(9)); // 显示信息  
                goto _show; // 跳转显示处理  
            } 
		// 如果选择的单元格是墙  
            if (CellField[VAR(DIFF) * LEN + VAR(LENGTH)].type == WALL) {  
                // 如果是墙，提示用户  
                printf("%s", MSG(14));  
                goto _input; // 回到输入处理  
            }  
            // 没有break语句，故fall through到下一个case  
        case 3: // 重新输入  
            goto _input; // 再次请求用户输入  
            break;  
        default: // 处理无效输入  
            printf("%s", MSG(15)); // 显示错误信息  
            goto _input; // 请求输入  
    } 
_show: // 显示当前地雷场状态的标记  
    VAR(J) = 0;  
    // 输出提示信息，显示行和列索引的标题  
    printf("\n%s %s", MSG(4), MSG(5));  

    // 遍历地雷场的每个单元格  
    for (VAR(I) = 1; VAR(I) <= LEN * LEN; VAR(I)++) {  
        // 每当到达新的一行，显示行号  
        if (!((VAR(I) - 1) % LEN)) {  
            VAR(J)++;  
            printf((VAR(J) >= 10) ? ("%d ") : ("0%d "), VAR(J)); // 行号打印  
        }  
    
        // 根据单元格的状态打印不同的标志  
        printf((CellField[VAR(I) - 1].status == UNSELECTED) ? ("*  ") : // 未选择状态  
               ((CellField[VAR(I) - 1].status == WALL) ? ("W  ") : // 墙壁状态  
                ((CellField[VAR(I) - 1].status == MARK) ? ("M  ") :  // 标记状态  
                ("%hd  "))), CellField[VAR(I) - 1].remain); // 安全状态，打印周围地雷数量  
        // 每当完成一行后换行  
        if (!(VAR(I) % LEN))  
            printf("\n");  
    }  
    
   

    // 如果还有未选择的安全格，继续请求输入  
    if (VAR(SAFECOUNT)) 
        goto _input;  
    else
	 printf("%s\n",MSG(11));
       goto _end;
  
_end: // 程序结束处理  
    // 释放动态分配的内存  
    free(vararr);  
    free(CellField);  
    free(MsgLst);  

    return 0; // 返回程序成功结束的状态  
}
