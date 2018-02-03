
#ifndef _H_CONST_H_
#define _H_CONST_H_
#include "cocos2d.h"
#define TIMESCALE 5.0f//时间比例尺
#define ACTION_BAR_TIME 4.0f//建筑物中进度条秒数
#define HERO_MOVE_SPEED 0.6f//地图中移动速度，min/px
#define EXSERCISE_DONE_HEROEXP (100)
#define EXSERCISE_DONE_GFEXP (100)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define ANALYTICS
//#define UMENG_SHARE
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define ANALYTICS
#endif

#define MAXCHAPTER (13)

static std::string acname[] = { "打猎", "采集", "砍伐", "挖掘" };
static std::string acdesc[] = {"","你徒手采集", "你徒手劈树", "你徒手采矿"};
static std::string acdesc2[] = { "", "费了好大力才获得了", "手掌都快劈烂了才获得了", "双手都磨破了才扣下来" };
static std::string acdesc1[] = {"", "你使用镰刀收割，毫不费力的获得", "你使用斧子劈树，毫不费力的获得", "你挥舞十字镐采矿，毫不费力的获得" };

static std::string herofightdesc[] = { "%s猛的跑上前去，对着%s耍了一通野球拳。"};
static std::string herofightdesc1[][5] = { {"%s挥舞着%s犹如黑虎下山，以迅猛之势击中%s，连连后退数步方稳住身形。", "%s把一根%s舞的密不透风，趁%s分神之际，直捣面门。", "%s高举手中的%s，三两步便靠近%s，一招将其打翻在地。", "%s手持%s将%s逼到角落，大喝一声“受死吧！”", "%s双手紧握%s虚晃一棍，接着“黑龙出洞”攻击%s胸口。"}, 
{"%s猛的挥出%s，气势凛冽，声如破竹，直奔%s面门而来。", "%s手握%s，冷冷的望着面前的%s，身形向前猛的一动，犹如毒蛇吐着芯子一般冲了过来。", "%s手持%s，刀刀抢攻，狠辣迅捷直逼%s要害。", "%s手握%s，招数精奇，攻击%s咽喉要害。", "%s手中%s犹如一条出海蛟龙冲向%s，刀风凛冽，刚猛强劲。"}, 
{"%s手中的%s轻轻一挑，拨开%s的武器，并反手一打，正中对方胸口。", "%s手中的%s化作一道耀眼眼光，直中%s。", "%s手握%s剑长三尺，剑气逼人，让%s眼花缭乱。", "%s从背后拿出%s深吸一口气后，暗中使上内力，朝着%s大力投射过去。", "只见%s青光闪动，一柄%s地刺出，腕抖剑斜，剑锋削向%s右颈。"} };

static std::string herofightdesc2[][5] = { { "%s被震的向后退了几步，差点倒在地上，受到%d点伤害。", "%s哇哇吐出一堆鲜血。受到%d点伤害。", "%s似乎受到了重击，已经摇摇欲坠，受到%d点伤害。", "%s单膝跪地，粗气连连，似乎耗费了很多力气，受到%d点伤害。", "%s以剑挡格，铮的一声响，武器相击，嗡嗡作声，长剑猛地击落，受到%d点伤害。" }, 
{"%s胸口中了一刀，鲜血砰的一声喷出来。受到%d点伤害。", "%s连忙收住身形，向左一滚，脸角擦破。受到%d点伤害。", "%s大叫一声不好，左腿已经中了一刀。受到%d点伤害。", "%s挥着兵器想去挡住这一刀，可惜为时已晚，刀口已经从他胸口划过。受到%d点伤害。", "%s面色剧变，一时定在原地，胸口一闷。受到%d点伤害。" },
{"%s喷出一口鲜血，单膝跪在地上，身体已经有些摇晃不稳。受到了%d点伤害。", "%s暗道不妙，未等来得及反应，已被胸口已被刺中。受到%d点伤害。", "%s翻滚勉强躲过一剑，却被反手一剑刺中，正中后腰上，翻倒在地。受到%d点伤害。", "%s已经连续挡住十余剑，此时已经大汗淋漓，手臂多处剑痕。受到%d点伤害。"} };

static std::string npcdodgedesc[] = {"%s见势不妙，急急忙忙向旁边一跃，躲开了这次攻击。", "%s急忙使出一招飞鹤冲天，跃起一米多高，竟然避开了这次攻击。", "%s已然吓出一身冷汗，一招追风步发挥到极致，竟然躲开了。"};
static std::string worfdodgedesc = "%s突然跑开，未命中目标。";

static std::string herododgedesc[] = {"%s这一招来势汹汹，%s只得收住招式，扭动身形，全力躲过这一击。", "%s这一招气势如虹，有如风雨大至之势，%s唯有拼进全力向后一跃，勉强躲过这一击。"};


static std::string herofightdesc3[] = { "拳头打在%s身上如同挠痒痒一样。受到%d点伤害。" };

static std::string bossfight[] = {"%s雷鸣闪电样迅速击中%s，硬生生的接住这一掌，受到%d点伤害。"};
static std::string bossfight1[] = { "%s雷鸣闪电样迅速击中%s，但有%s挡了一下，虽然没有大碍，但还是受到%d点伤害。" };

static std::string herofightworfword[] = { "%s面对眼前的%s，凝神运气，迅速踢出一脚。 " };
static std::string herofightworfword2[] = { "%s被突如其来的一脚踢中脊梁，向后滚了好几圈。受到%d点伤害 " };
static std::string worffight[] = { "%s翻起身朝%s冲了过来，一口咬在了腿上。受到%d点伤害。" };

static std::string herofightRabbitword[] = { "%s趁%s吃草的时候，一个马步上前，狠狠一脚踢到它的脑袋上。" };
static std::string herofightRabbitword2[] = { "%s四脚朝天，飞了出去。受到%d伤害" };
static std::string rabbitfightword[] = { "%s蹦蹦跳跳的跑过来在%s脚上咬了一口。受到%d伤害" };

static std::string herousewgfight[][4] = { {"%s使出一招失传已久的%s，%s吓得脸色发白，急忙向后跑去。", "%s双手置于胸前，微闭双眼，长啸一声，使出一招%s化身一道长虹，直射%s胸前。", "%s突然双臂提起，一股劲风猛然扑出，%s如同一道闪电，劈向%s身前。", "%s施展一招%s，犹如排山倒海之势，直击%s面门。"}, 
{"%s对着%s使出一招%s，一招%s舞的生龙活虎，把%s打的连连后退好几步。", "%s全神应对%s突然施展一招%s，%s乃是威力极大的一招，把%s打的哇哇吐了一地血。", "%s仰头哈哈大笑，完全不把%s瞧在眼里，突然一招%s，%s早已练得登峰造极，%s只得勉强接着这一招。", "%s怒火中烧，暗中运起内力，朝着%s到头打去，一招%s拍出，%s来势刚烈之极，%s只得运起内功全力挡住这一招。"} };

static std::string bossgetherowg[] = {"%s后退数步方稳住身形，深吸一口气，只觉体内真气大乱，显然是受了很重的内伤。受到%d点伤害。", "%s背后狠狠的挨了一下，站立不稳，摔了一个狗啃屎，趴在地上半天爬不起来。受到%d点伤害。", "%s一看实在躲不过这一招，只能硬着头皮举拳相向，两招相交，震得双臂发麻。受到%d点伤害。", "%s眼见躲不过去，心中暗骂了一句该死，只好就地一滚，浑身沾满了泥。受到%d点伤害。"};


static std::string heroname[] = { "小虾米", "三少爷", "江小白", "阿青" };
static std::string herodesc[] = { "名门之后，因家族武功被邪教窥觑，惨遭灭门，只剩孤身一人", "从未来穿越而来的特种兵，为了回到属于自己的时代而被迫行走江湖", "恶人谷出身，习得各大恶人的看家本领，但本性善良，被送出谷历练", "情郎被邪教蛊惑，为寻回情郎，孤身一人闯荡江湖！" };
static int heroprice[] = { 0, 6, 12, 30 };

static std::string payCode[] = { "", "cwx010", "cwx011", "cwx013", "cwx018", "cwx019", "cwx020", "cwx021", "cwx026", "cwx022", "cwx023","cwx024","cwx025"};
static int goldcount[] = {60, 140, 400, 1080, 2388};
static int vipgoldcount[] = { 10,20, 50 };

static int buyprice[] = { 0, 6, 12, 30, 6,12, 30, 68, 128, 6, 30, 68, 18 };

static std::string tipswords0[] = { "温度过低时会加速你的各项属性消耗，记得保持好温度！" };
static std::string tipswords1[] = { "后山的狼和兔子是个练基本功的好对手", "季节的变换会影响温度与后山的资源动态！" };

static std::string tipswords2[] = { "精神不足时会降低饥饿度的恢复效果", "精神不足时会降低药物的治疗效果", "精神不足时会降低睡眠的恢复效果", "饥饿会让你的战斗力直线下降，所以战斗前一定要吃饱饭！",
"受伤后要尽快使用对应的药物进行治疗", "受伤时如果不立即治疗，伤势会越来越严重", "受伤严重时会影响你的武力发挥",
"受伤严重时会加速你的生命消耗", "后山上有可供你活下去的各种资源", "合理的利用后山资源可以让你的生活更好", "好的休息可以恢复你已经消耗的生命值",
"每个武林中人都有属于自己的独门武功，去打败他们获得武功吧！"
};
static std::string tipswords3[] = { "没有最强的武器，只有最强的人！", "品质从低到高的颜色依次为：无、绿、蓝、紫、橙", "结交好友可获得属性的加成，好友多多益善！", "武道大会复活后可继续挑战，不需要重新开始。",
"可以在客栈里跟韦小宝进行交易，每日交易的物品都有不同哦~", "结交好友可获得属性的加成，好友多多益善！", "拜师可以获得专属武功的加成，拜师只能选择一个哦！",
"结交好友后会触发随机事件，千万要谨慎选择好友！", "在山间行走时有机会遇到藏宝悬崖，可遇不可求哦！"
};

static std::string exersiceDesc = "建议退出游戏前闭关，闭关期间无法进行其它行为（拥有分身符可同时）。闭关时间为现实时间，请选择合理时间进行闭关！";
static std::string qudesc[] = { "入门", "一流", "上乘", "传世", "绝世" };
static cocos2d::Color4B qucolor[] = { cocos2d::Color4B(76, 54, 51, 255), cocos2d::Color4B(27, 141, 0, 255), cocos2d::Color4B(27, 84, 155, 255), cocos2d::Color4B(171, 34, 210, 255), cocos2d::Color4B(216, 151, 82, 255) };
static std::string wxbinmap[] = {"m3-1","m4-1", "m7-2"}; 

static std::string dgqbinmap[] = { "m7-3", "m3-4", "m4-9", "m6-3", "m7-12", "m2-3", "m7-8", "m10-1", "m11-3" };
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//static std::string QQNUM[] = { "621564945", "593243749" };
//#else
//static std::string QQNUM[] = { "641759306" };
//#endif
static std::string LOGINREWARDS[] = { "80020", "g10", "12010", "80050", "g20", "81010", "82005" };

typedef enum
{
	TIMEGIFT = sizeof(heroprice) / sizeof(heroprice[0]) + sizeof(goldcount) / sizeof(goldcount[0]) + sizeof(vipgoldcount) / sizeof(vipgoldcount[0]),
	GIFTMAX

}GIFTTYPE;
#endif // _H_CONST_H_
