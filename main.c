#include<stdio.h>
#define NUM 5

typedef struct 
{
    char brand[10];//商品名字
    char id[10];//商品编号
    float in_price;//进价
    float out_price;//卖价
    int storage;//库存数量
}Goods;

Goods goods[NUM];//用来存储商品信息 最多为NUM条

int menu(void);
void establish(void)
{
    FILE *fp;
    printf("请依次输入货物信息:\n");
    printf("---------------------------------------------\n");
    //初始化商品信息
    for (int i = 0; i < NUM; i++)
    {
        printf("请输入商品的名字:\n");
        fflush(stdin);
        scanf("%s",goods[i].brand);
        printf("请输入商品的编号:\n");
        fflush(stdin);
        scanf("%s",goods[i].id);
        printf("请输入商品的进价:\n");
        fflush(stdin);
        scanf("%s",&goods[i].in_price);
        printf("请输入商品的卖价:\n");
        fflush(stdin);
        scanf("%s",&goods[i].out_price);
        printf("请输入商品的数量:\n");
        fflush(stdin);
        scanf("%s",&goods[i].storage);
        printf("\n");
        }
    //判断文件是否创建成功
    if((fp = fopen("goods","w")) == NULL)
    {
        printf("创建失败\n");
        return 0;
    }
    //把goods数组的内容写入文件
    fwrite(goods,sizeof(Goods),NUM,fp);
    //关闭文件
    fclose(fp);
}

int menu(void)
{
    int select;
    printf("请选择数字进行操作：\n");
    printf("1、建立库存信息\n");
    printf("2、显示库存信息\n");
    printf("3、购物车\n");
    printf("4、结算\n");
    printf("5、退出\n");
    printf("请选择对应数字1-5:");
    while(1)
    {
        scanf("%d",&select);
        if(select<1 || select >5)
        {
            printf("请重新输入有效数字");
        }
        else
        {
            break;
        }
    }
    return select;
}

int main(void)
{
    printf("***********************************************\n");
    printf("\t欢迎使用超市购物系统\n");
    printf("***********************************************\n");
    switch (menu())
    {
    case 1:establish();break;
    case 2:printf("现在的库存信息如下\n");break;
    case 3:printf("您的购物车中有\n");break;
    case 4:printf("正在为您结算\n");break;
    case 5:printf("欢迎下次光临\n");break;
    }
    
    
    return 0;
}
