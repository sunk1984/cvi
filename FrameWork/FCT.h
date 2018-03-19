
#define PORT_MERAK  0
#define PORT_DUT    1
#define PORT_AUX    2
#define PORT_PRT    3


extern char info_text[200];
extern char item_text[4000];
extern char sub_item_text[4000];

extern int mainPanel;
extern int setupPanel;
extern int resultPanel;

extern char result_str[2][10];

extern void delay_ms(int ms);
extern void LoadAllPorts(void);
extern void PoolingAllPorts(void);

