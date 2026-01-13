#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NUM 5

typedef struct 
{
    char brand[10];//商品名字
    char id[10];//商品编号
    float in_price;//进价
    float out_price;//卖价
    int storage;//库存数量
}Goods;

typedef struct Item
{
    Goods wanted;//商品
    int amount;//购物数量
    struct Item *next;
}Scart;

Goods goods[NUM];//用来存储商品信息 最多为NUM条
Scart *cart;//购物车头指针
void dis_all(void);//展示当前库存
int menu(void);//开始菜单界面
void establish(void);//建立超市初始商品信息
void shop_cart(void);//购物车功能实现
int cart_menu(void);//购物车菜单
void add(void);//添加商品
void delete();//删除商品
void display(void);//显示购物列表
void cal(void);//结算功能的实现

void cal(void)
{
    float total = 0,pay=0;
    Scart *p;
    FILE *fp;
    printf("以下是购物清单：\n");
    display();//显示购物车内容
    if((fp = fopen("goods","rb")) == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    for (int i = 0;(fread(goods+i,sizeof(Scart),1,fp)) != 0; i++);
    fclose(fp);
    p = cart;
    while(p != NULL)
    {
        total += p->wanted.out_price * p->amount;
        for (int  i = 0; strcmp(goods[i].id,p->wanted.id) != 0; i++)
        {
           goods[i].storage -= p->amount;
        }
        p = p->next;   
    }
    printf("总计：%7.2f\n",total);
    printf("\n输入支付的金额:");
    scanf("%f",&pay);
    printf("实际支付：%7.2f\t\t 找零：%7.2f\n",pay,pay-total);
    if((fp = fopen("goods","wb")) == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    fwrite(goods,sizeof(Goods),NUM,fp);
    fclose(fp);
    
}
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
        scanf("%f",&goods[i].in_price);
        printf("请输入商品的卖价:\n");
        fflush(stdin);
        scanf("%f",&goods[i].out_price);
        printf("请输入商品的数量:\n");
        fflush(stdin);
        scanf("%d",&goods[i].storage);
        printf("\n");
    }
    //判断文件是否创建成功
    if((fp = fopen("goods","w")) == NULL)
    {
        printf("创建失败\n");
        return;
    }
    //把goods数组的内容写入文件
    fwrite(goods,sizeof(Goods),NUM,fp);
    //关闭文件
    fclose(fp);
}
void dis_all(void)
{
    //清空前面显示的内容
    //在cmd上才有效，在vs上的terminal无效
    //system("cls");


    FILE *fp;
    fp = fopen("goods","r");
    for (int i = 0;(fread(goods+i,sizeof(Goods),1,fp)) != 0; i++)
    {
        printf("---------------------------------------------\n");
        printf("商品的编号:%s\n",goods[i].id);
        printf("商品的名字:%s\n",goods[i].brand); 
        printf("商品的价格:%.2f\n",goods[i].out_price);
        printf("商品的数量:%d\n",goods[i].storage);
        printf("\n");
    }
    fclose(fp);
}
void shop_cart(void)
{
    while (1)
    {
        int num = cart_menu();
        switch (num)
        {
            case 1:display();break;
            case 2:add();break;
            case 3:printf("删除成功，您现在的购物车中有\n");break;
            case 4:printf("正在退出\n");break;
        }
        printf("\n");
        if(num == 4) break;
    }
}
int cart_menu(void)
{
    printf("\n");
    int select;
    printf("请选择操作:\n");
    printf("---------------------------------\n");
    printf("1、显示当前购物列表\n");
    printf("2、添加商品\n");
    printf("3、删除商品\n");
    printf("4、退出\n");
    printf("---------------------------------\n");

    while(1)
    {
        printf("请选择操作：");
        scanf("%d",&select);
        if(select<1 || select>4)
        {
            printf("无效数字，请重新输入\n");
        }
        else
        {
            break;
        }
    }
}
//add 有点小问题 会吞字符
//已改好 通过清空输入缓冲区的办法解决
void add(void)
{
    FILE *fp;
    int i;
    char str[20];//获取商品名字/编号的字符串
    int n;//顾客想要商品的数量
    char choice,choice2;
    Scart *p,*p1;
    int ch;
    do
    {
        printf("请输入所需物品的名称或者编号：");
        //fflush(stdin);
        //scanf("%s",str);
        while ((ch = getchar()) != '\n' && ch != EOF);
        gets(str);
        if((fp = fopen("goods","r")) == NULL)
        {
            printf("打开文件失败！\n");
            continue;
        }
        for(i=0;fread(goods+i,sizeof(Goods),1,fp);i++)
        {
            if((strcmp(goods[i].brand,str) == 0 || strcmp(goods[i].id,str) == 0 ) && goods[i].storage>0)
            {
                printf("找到所需物品\n");
                printf("----------------------------------\n");
                printf("编号\t名字\t价格 \n");
                printf("%s%12s%9.2f\n",goods[i].id,goods[i].brand,goods[i].out_price);
                printf("请输入所需物品的数量：");
                scanf("%d",&n);
                if(n > goods[i].storage)
                {
                    printf("库存不足！");
                    break;
                }
                printf("是否添加进购物车?(Y/N):\n");
                while ((ch = getchar()) != '\n' && ch != EOF);
                //fflush(stdin);
                choice = getchar();
                if(choice == 'Y'||choice == 'y')
                {
                    p1 = (Scart*)malloc(sizeof(Scart));
                    if(p1==NULL) printf("内存申请失败");
                    p1->amount = n;
                    p1->wanted = goods[i];
                    p1->next = NULL;
                    p = cart;
                    if(cart == NULL) cart = p1;
                    else
                    {
                        while(p->next != NULL)
                            p= p->next;
                        p1->next = p->next;
                        p->next = p1;
                    }
                }
                break;
            }
        }
        if(i == NUM)
        {
            printf("未找到所需物品\n");
        }
        fclose(fp);
        printf("是否继续购物?(Y/N):\n");
        // fflush(stdin);
        while ((ch = getchar()) != '\n' && ch != EOF);
        choice2 = getchar();
        //scanf("%c",&choice2);
    } while (choice2 == 'Y' || choice2 == 'y');
    
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
            printf("无效数字，请重新输入");
        }
        else
        {
            break;
        }
    }
    return select;
}
//显示购物列表
void display(void)
{
    Scart *p = cart;
    if(p == NULL)
    {
        printf("购物车为空！\n");
        return;
    }
    while(p != NULL)
    {
        printf("-----------------------------\n");
        printf("编号\t名字\t单价\t数量\n");
        printf("%s%11s%9.2f%7d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
        p=p->next;
    }
}

int main(void)
{
    printf("***********************************************\n");
    printf("\t欢迎使用超市购物系统\n");
    printf("***********************************************\n");
    while (1)
    {
        int num = menu();
        switch (num)
        {
            case 1:establish();break;
            case 2:dis_all();break;
            case 3:shop_cart();break;
            case 4:cal();break;
            case 5:printf("感谢使用,欢迎下次光临\n");break;
        }
        if(num == 5) break;
        printf("\n");
    }
    

    
    
    return 0;
}
