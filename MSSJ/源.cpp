#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<iterator>
#include<algorithm>
#include<queue>
using namespace std;
#define WARRIOR_NUM  5
#define WEAPON_NUM  3
#define w_c deque<CWarrior*>
 struct MyClock {
	int hour;
	int minute;
	void Initial() {
		hour = 0;
		minute = 0;
	}
	int tran_minu() {
		return 60 * hour + minute;
	}
} ;

ostream& operator<<(ostream& o, MyClock cl) {
	o << setw(3) << setfill('0') << cl.hour << ":" << setw(2) << setfill('0') << cl.minute << " ";
	return o;
}

class CWarrior;
class weapon {
protected:
	int mKind;
	int mForce;
public:
	static string Weapon_names[WEAPON_NUM];
	virtual void wp_new_force(int wa_stg) = 0;//
	int& get_kind() {
		return mKind;
	}
	int get_force() const {
		return mForce;
	}
	friend CWarrior;
};
bool less_weapon(weapon* p1, weapon* p2) {
	return p1->get_kind() < p2->get_kind();
}
class sword :public weapon {
public:
	sword(int f) {
		mKind = 0;
		mForce = f * 2 / 10;
	}
	virtual void wp_new_force(int wa_stg) {
		mForce = wa_stg * 2 / 10;
	}
};
class bomb :public weapon {
public:
	bomb(int f) {
		mKind = 1;
		mForce = f * 4 / 10;
	}
	virtual void wp_new_force(int wa_stg) {
		mForce = wa_stg * 4 / 10;
	}
};
class arrow :public weapon {
public:
	arrow(int f) {
		mKind = 3;//没用过
		mForce = f * 3 / 10;
	}
	virtual void wp_new_force(int wa_stg) {
		mForce = wa_stg * 3 / 10;
	}
};
class CHeadquarter;
class Dragon;
class Ninja;
class Iceman;
class Lion;
class Wolf;
class CWarrior {
protected:
	int nNo;
	int KindNo;
	CHeadquarter* pHeadquarter;
	int elements;//生命值
	deque<weapon*> dq_wp;
public:
	static string names[WARRIOR_NUM];
	static int InitialLifeValue[WARRIOR_NUM];
	static int Strength[WARRIOR_NUM];//本体攻击力
	static vector<w_c> ci_wa;
	static MyClock time;
	static bool game_over;
	static int lion_loss_loyalty;
	static void Initial(int n, int k);
	static void call_destructor();
	CWarrior(CHeadquarter* p, int nNo_, int KindNo_);
	virtual void PrintResult();
	virtual bool escape();
	virtual ~CWarrior();
	virtual int wa_go_ahead();
	int Getcolor();
	void PrintGoahead(int n);
	void wa_theft(CWarrior* p2, int c_n);
	int beat(CWarrior* p);
	void pick_up_wp(deque<weapon*>& wp);
	void report();
	friend void ci_theft(w_c& dq, int c_n);
	friend void ci_battle(w_c& dq, int c_n);
};
class CHeadquarter {
private:
	int totalLifeValue;
	bool bStopped;
	int totalWarriorNum;
	int color;
	int curMakingSeqIdx;
public:
	static int makingSeq[2][WARRIOR_NUM];//0 red 1blue
	void Init(int color_, int lv);
	int Produce();
	void GetColor(string& s);
	bool GetStop();
	int Get_LifeValue();
	friend class CWarrior;
	friend Lion;
};
weapon* weapon_produce(int k1, int k2) {
	weapon* p = NULL;
	switch (k1) {
	case 0:
		p = new sword(CWarrior::Strength[k2]);
		break;
	case 1:
		p = new bomb(CWarrior::Strength[k2]);
		break;
	case 2:
		p = new arrow(CWarrior::Strength[k2]);
		break;
	default:
		break;
	}
	return p;
}
class Dragon :public CWarrior {
public:
	Dragon(CHeadquarter* p, int nNo_, int KindNo_) :CWarrior(p, nNo_, KindNo_) {
		int k = nNo % 3;
		weapon* p_w = weapon_produce(k, KindNo);
		dq_wp.push_back(p_w);
	}
};
class Ninja :public CWarrior {
public:
	Ninja(CHeadquarter* p, int nNo_, int KindNo_) :CWarrior(p, nNo_, KindNo_) {
		int k1 = nNo % 3;
		weapon* p_w = weapon_produce(k1, KindNo);
		dq_wp.push_back(p_w);
		int k2 = (nNo + 1) % 3;
		p_w = weapon_produce(k2, KindNo);
		if (k2 > k1)//编号大的放后面
			dq_wp.push_back(p_w);
		else
			dq_wp.push_front(p_w);
	}
};
class Iceman :public CWarrior {
public:
	Iceman(CHeadquarter* p, int nNo_, int KindNo_) :CWarrior(p, nNo_, KindNo_) {
		int k = nNo % 3;
		weapon* p_w = weapon_produce(k, KindNo);
		dq_wp.push_back(p_w);
	}
	virtual int wa_go_ahead() {
		int loss_lifevalue = elements / 10;
		elements -= loss_lifevalue;
		return CWarrior::wa_go_ahead();
	}
};
class Lion :public CWarrior {
private:
	int loyalty;
public:
	Lion(CHeadquarter* p, int nNo_, int KindNo_) :CWarrior(p, nNo_, KindNo_) {
		loyalty = p->totalLifeValue;//totalLifeValue 为降生后值
		int k = nNo % 3;
		weapon* p_w = weapon_produce(k, KindNo);
		dq_wp.push_back(p_w);
	}
	virtual void PrintResult() {
		CWarrior::PrintResult();
		cout << "Its loyalty is " << loyalty << endl;
	}
	virtual bool escape();
	virtual int wa_go_ahead() {
		loyalty -= lion_loss_loyalty;
		return CWarrior::wa_go_ahead();
	}
};
class Wolf :public CWarrior {
public:
	Wolf(CHeadquarter* p, int nNo_, int KindNo_) :CWarrior(p, nNo_, KindNo_) {}
};
CWarrior::CWarrior(CHeadquarter* p, int nNo_, int KindNo_) :
	nNo(nNo_), KindNo(KindNo_), pHeadquarter(p), elements(InitialLifeValue[KindNo]) {
	if (p->color) {//blue
		ci_wa[ci_wa.size() - 1].push_back(this);
	}
	else {
		ci_wa[0].push_back(this);
	}
}
CWarrior::~CWarrior() {
	for (int i = 0; i < dq_wp.size(); i++)
		delete dq_wp[i];
}
void CWarrior::PrintResult() {
	string str_color;
	pHeadquarter->GetColor(str_color);
	cout << time << str_color << " " << names[KindNo] << " " << nNo << " born" << endl;
}
void CWarrior::Initial(int n, int k) {
	ci_wa = vector<w_c>(n + 2);
	time.Initial();
	game_over = false;
	lion_loss_loyalty = k;
}
void CWarrior::call_destructor() {
	for (int i = 0; i < ci_wa.size(); i++) {
		for (int j = 0; j < ci_wa[i].size(); j++)
			delete ci_wa[i][j];
	}
}
bool CWarrior::escape() { return false; }
int CWarrior::wa_go_ahead() {//除Iceman Lion
	if (pHeadquarter->color) {// 1 blue
		return -1;
	}
	else
		return 1;
}
int CWarrior::Getcolor() {
	return pHeadquarter->color;
}
void CWarrior::PrintGoahead(int n) {
	string str_color, enermy_color;
	pHeadquarter->GetColor(str_color);
	if (str_color == "red")
		enermy_color = "blue";
	else
		enermy_color = "red";
	if (n == 0 || n == ci_wa.size() - 1) {
		cout << time << str_color << " " << names[KindNo] << " " << nNo << " reached " << enermy_color
			<< " headquarter with " << elements << " elements and force " << Strength[KindNo] << endl;
		cout << time << enermy_color << " headquarter was taken" << endl;
		game_over = true;
	}
	else
		cout << time << str_color << " " << names[KindNo] << " " << nNo << " marched to city " << n << " with "
		<< elements << " elements and force " << Strength[KindNo] << endl;
}
void CWarrior::wa_theft(CWarrior* p2, int c_n) {//只抢一种
	deque<weapon*>& dq_wp2 = p2->dq_wp;//对手武器库
	if (dq_wp2.size() == 0)
		return;
	int kd = dq_wp2.front()->get_kind();
	int count = 0;
	while (dq_wp.size() != 10 && dq_wp2.size() != 0) {
		weapon* p = dq_wp2.front();
		if (p->get_kind() != kd) {
			if (p->get_kind() == 3 && kd == 2)
				dq_wp2.pop_front();
			else
				break;
		}
		else if (p->get_kind() == 2) {//旧arrow
			weapon* p_ = dq_wp2.back();//最后一个
			if (p_->get_kind() == 2) {//全是旧的
				dq_wp2.pop_front();
			}
			else {//最后新的
				p = p_;
				dq_wp2.pop_back();
			}
		}
		else//正常
			dq_wp2.pop_front();
		p->wp_new_force(Strength[KindNo]);
		dq_wp.push_back(p);//最后排序
		count++;
	}
	if (count) {
		sort(dq_wp.begin(), dq_wp.end(), less_weapon);
		string str_color, enermy_color;
		pHeadquarter->GetColor(str_color);
		if (kd == 3)
			kd = 2;
		if (str_color == "red")
			enermy_color = "blue";
		else
			enermy_color = "red";
		cout << time << str_color << " " << names[KindNo] << " " << nNo << " took " << count << " "
			<< weapon::Weapon_names[kd] << " from " << enermy_color << " " << names[p2->KindNo] << " " << p2->nNo
			<< " in city " << c_n << endl;
	}//if(positive)
}
bool if_fake(const deque<weapon*>& p) {
	if (p.size() == 0)
		return true;
	else {
		for (auto i = p.begin(); i != p.end(); i++)
			if ((*i)->get_kind() != 0 || (*i)->get_force() != 0)
				return false;
		return true;
	}
}
int CWarrior::beat(CWarrior* p) {//1、双方无武器；2、武器状态不在变化（sword） 且 攻击力为0；
	//武器用后回收，sword 0, arrow 3
	deque<weapon*>& be_beated_wp = p->dq_wp;//对手武器库
	if (dq_wp.size() == 0 && be_beated_wp.size() == 0)
		return 1;
	if (if_fake(dq_wp) && if_fake(be_beated_wp))
		return 2;
	if (dq_wp.size() == 0)//
		return 0;
	weapon* wp = dq_wp.front();
	dq_wp.pop_front();
	p->elements -= wp->get_force();
	if (wp->get_kind() == 0)
		dq_wp.push_back(wp);
	else if (wp->get_kind() == 1) {//bomb 使用者ninja(1)不受伤
		if (KindNo != 1)//*******************************************************重点
			elements -= wp->get_force() / 2;
	}
	else if (wp->get_kind() == 3) {//全新arrow
		wp->get_kind() = 2;
		dq_wp.push_back(wp);
	}
	return 0;
}
void CWarrior::pick_up_wp(deque<weapon*>& dq_wp2) {
	sort(dq_wp2.begin(), dq_wp2.end(), less_weapon);
	while (dq_wp.size() != 10 && dq_wp2.size() != 0) {
		weapon* p = dq_wp2.front();
		if (p->get_kind() != 2) {//不是旧arrow
			dq_wp2.pop_front();
		}
		else {//首个wp为 旧的
			weapon* p_ = dq_wp2.back();//最后一个
			if (p_->get_kind() == 2) {//全是旧的
				dq_wp2.pop_front();
			}
			else {//最后新的
				p = p_;
				dq_wp2.pop_back();
			}
		}
		p->wp_new_force(Strength[KindNo]);
		dq_wp.push_back(p);//最后排序
	}
	sort(dq_wp.begin(), dq_wp.end(), less_weapon);
}
void CWarrior::report() {
	const deque<weapon*>& wp = dq_wp;
	int wa_kind_no[3] = { 0,0,0 };
	for (int i = 0; i != wp.size(); i++) {
		int k = wp[i]->get_kind();
		if (k == 3)
			wa_kind_no[2]++;
		else
			wa_kind_no[k]++;
	}
	string str_color;
	pHeadquarter->GetColor(str_color);
	cout << time << str_color << " " << names[KindNo] << " " << nNo << " has " << wa_kind_no[0] << " sword "
		<< wa_kind_no[1] << " bomb " << wa_kind_no[2] << " arrow and " << elements << " elements" << endl;
}
bool Lion::escape() {
	if (loyalty <= 0) {
		string str_color;
		pHeadquarter->GetColor(str_color);
		cout << time << str_color << " lion " << nNo << " ran away" << endl;
		delete this;
		return true;
	}
	return false;
}
void CHeadquarter::Init(int color_, int lv) {
	color = color_;
	totalLifeValue = lv;
	totalWarriorNum = 0;
	bStopped = false;
	curMakingSeqIdx = 0;
}
int CHeadquarter::Produce() {
	if (bStopped == true)
		return 0;
	int KindNo = makingSeq[color][curMakingSeqIdx];
	if (CWarrior::InitialLifeValue[KindNo] > totalLifeValue) {
		bStopped = true;
		return 0;
	}
	totalLifeValue -= CWarrior::InitialLifeValue[KindNo];
	curMakingSeqIdx = (curMakingSeqIdx + 1) % WARRIOR_NUM;
	CWarrior* p;
	switch (KindNo) {
	case 0:
		p = new Dragon(this, ++totalWarriorNum, KindNo);
		p->PrintResult();
		break;
	case 1:
		p = new Ninja(this, ++totalWarriorNum, KindNo);
		p->PrintResult();
		break;
	case 2:
		p = new Iceman(this, ++totalWarriorNum, KindNo);
		p->PrintResult();
		break;
	case 3:
		p = new Lion(this, ++totalWarriorNum, KindNo);
		p->PrintResult();
		break;
	case 4:
		p = new Wolf(this, ++totalWarriorNum, KindNo);
		p->PrintResult();
		break;
	}
	return 1;
}
void CHeadquarter::GetColor(string& s) {
	if (color == 0)
		s = "red";
	if (color == 1)
		s = "blue";
}
bool CHeadquarter::GetStop() {
	return bStopped;
}
int CHeadquarter::Get_LifeValue() {
	return totalLifeValue;
};
void destructor_warrior(CWarrior* p) {
	delete p;
}
void if_ciwa_esc(w_c& wa_i) {//红蓝顺序已调好
	if (wa_i.size() == 2) {
		CWarrior* i = wa_i.front();
		if (i->escape()) {
			wa_i.pop_front();
			i = wa_i.front();//blue
		}
		else
			i = wa_i.back();;//blue
		if (i->escape())
			wa_i.pop_back();
	}
	else if (wa_i.size() == 1) {
		CWarrior* i = wa_i.front();
		if (i->escape())
			wa_i.pop_front();
	}
}
void fun1(vector<w_c>::iterator i, CWarrior* j) {
	if (j->wa_go_ahead() == 1) {//red
		CWarrior* temp = j;
		i->pop_front();
		auto i_next = i + 1;
		i_next->push_front(temp);
	}
}
void red_go_ahead(vector<w_c>::iterator i) {//暂不判断是否进入hq;每个元素属性调整，但只要红色前进
	if (i->size() == 2) {
		CWarrior* j = i->front();//red
		fun1(i, j);//j前进,剩余元素也要wa_go_ahead()
		j = i->front();
		fun1(i, j);
	}
	else if (i->size() == 1) {
		CWarrior* j = i->front();
		fun1(i, j);
	}
}
void fun2(vector<w_c>::iterator i, CWarrior* j) {
	CWarrior* temp = j;
	i->pop_back();
	auto i_pre = i - 1;
	i_pre->push_back(temp);
}
void blue_go_ahead(vector<w_c>::iterator i) {
	if (i->size() == 2) {
		CWarrior* j = i->back();
		fun2(i, j);
	}
	else if (i->size() == 1) {
		CWarrior* j = i->back();
		if (j->Getcolor())
			fun2(i, j);
	}
}
void ci_theft(w_c& dq, int c_n) {
	CWarrior* p_wa1 = dq.front();
	CWarrior* p_wa2 = dq.back();
	if (p_wa1->KindNo == 4 && p_wa2->KindNo != 4) //p1 wolf
		p_wa1->wa_theft(p_wa2, c_n);
	else if (p_wa2->KindNo == 4 && p_wa1->KindNo != 4)
		p_wa2->wa_theft(p_wa1, c_n);
}
void ci_battle(w_c& dq, int c_n) {
	//战斗结束条件：1、双方无武器；2、武器状态不在变化（sword） 且 攻击力为0；3、一方或双方死亡；要点：1、ninja不受bomb伤害
	CWarrior* beat = NULL;
	CWarrior* be_beated = NULL;
	int t = c_n;
	int symbol = 0;//标志 -1（有死者） 0(正常，继续打)  1（双方武器库空） 2（没有都空但是 是 假的（sword 0））
	while (dq.front()->elements > 0 && dq.back()->elements > 0) {// both alive 
		if (t % 2) {//odd
			beat = dq.front();
			be_beated = dq.back();
		}
		else {
			beat = dq.back();
			be_beated = dq.front();
		}
		symbol = beat->beat(be_beated);//beat 之前查看武器库
		if (symbol)//双方均alive,武器有问题
			break;
		t++;
	}
	cout << CWarrior::time;
	if (symbol == 0) {
		if (dq.front()->elements <= 0 && dq.back()->elements <= 0) {//both dead
			cout << "both red " << CWarrior::names[dq.front()->KindNo] << " " << dq.front()->nNo << " and blue "
				<< CWarrior::names[dq.back()->KindNo] << " " << dq.back()->nNo << " died in city " << c_n << endl;
			delete dq.front();
			delete dq.back();
			dq.clear();
			return;
		}//both dead
		else {//one dead,the other alive
			CWarrior* dead = NULL;
			CWarrior* alive = NULL;
			if (dq.front()->elements <= 0) {
				dead = dq.front();
				alive = dq.back();
			}
			else {
				dead = dq.back();
				alive = dq.front();
			}
			string alive_color, dead_color;
			dead->pHeadquarter->GetColor(dead_color);
			alive->pHeadquarter->GetColor(alive_color);
			cout << alive_color << " " << CWarrior::names[alive->KindNo] << " " << alive->nNo << " killed " << dead_color
				<< " " << CWarrior::names[dead->KindNo] << " " << dead->nNo << " in city " << c_n
				<< " remaining " << alive->elements << " elements" << endl;
			deque<weapon*>& dead_wp = dead->dq_wp;
			alive->pick_up_wp(dead_wp);//pick_up_wp() 中排序
			delete dead;
			if (alive->Getcolor())
				dq.pop_front();
			else
				dq.pop_back();
			if (alive->KindNo == 0) //dragon
				cout << CWarrior::time << alive_color << " " << CWarrior::names[alive->KindNo] << " " << alive->nNo
				<< " yelled in city " << c_n << endl;
			return;
		}//else(one dead,the other alive)
	}//if (有dead)
	// both alive symbol 为1.武器库空;2 all sword
	cout << "both red " << CWarrior::names[dq.front()->KindNo] << " " << dq.front()->nNo << " and blue "
		<< CWarrior::names[dq.back()->KindNo] << " " << dq.back()->nNo << " were alive in city " << c_n << endl;
	if (dq.front()->KindNo == 0)
		cout << CWarrior::time << "red " << CWarrior::names[dq.front()->KindNo] << " " << dq.front()->nNo
		<< " yelled in city " << c_n << endl;
	if ((dq.back()->KindNo == 0))
		cout << CWarrior::time << "blue " << CWarrior::names[dq.back()->KindNo] << " " << dq.back()->nNo
		<< " yelled in city " << c_n << endl;
}
string CWarrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
// 对应关系 ：和输入初始生命值顺序相同     0         1        2      3      4
string weapon::Weapon_names[WEAPON_NUM] = { "sword","bomb","arrow" };//3 没用过的
// 对应关系                                      0       1      2      
int CWarrior::InitialLifeValue[WARRIOR_NUM];
int CWarrior::Strength[WARRIOR_NUM];
bool  CWarrior::game_over;
vector<w_c> CWarrior::ci_wa;
MyClock CWarrior::time;
int CWarrior::lion_loss_loyalty;
int CHeadquarter::makingSeq[2][WARRIOR_NUM] = { {2,3,4,1,0},{3,0,1,2,4} };//0 R ;1 B 
int main() {
	int case_no;//测试数据组数
	int m, n, k, t;
	CHeadquarter RedHead, BlueHead;
	scanf("%d", &case_no);
	int nCaseNo = 1;
	while (case_no--) {
		printf("Case %d:\n", nCaseNo++);
		cin >> m >> n >> k >> t;
		for (int i = 0; i < WARRIOR_NUM; i++)
			scanf("%d", &CWarrior::InitialLifeValue[i]);
		for (int i = 0; i < WARRIOR_NUM; i++)
			scanf("%d", &CWarrior::Strength[i]);
		CWarrior::Initial(n, k);
		RedHead.Init(0, m);
		BlueHead.Init(1, m);
		while (true) {
			if (CWarrior::time.tran_minu() <= t) {//1 produce
				if (!RedHead.GetStop() || !BlueHead.GetStop()) {
					RedHead.Produce();
					BlueHead.Produce();
				}//if
				CWarrior::time.minute = 5;
			}
			else
				break;
			if (CWarrior::time.tran_minu() <= t) {//2 lion escape
				for_each(CWarrior::ci_wa.begin(), CWarrior::ci_wa.end(), if_ciwa_esc);
				CWarrior::time.minute = 10;
			}
			else
				break;
			if (CWarrior::time.tran_minu() <= t) {//3 go_head 
				for (vector<w_c> ::iterator i = CWarrior::ci_wa.end(); i != CWarrior::ci_wa.begin(); i--) {//从后往前
					auto j = i - 1;
					red_go_ahead(j);
				}
				for (vector<deque<CWarrior*>>::iterator i = CWarrior::ci_wa.begin(); i != CWarrior::ci_wa.end(); i++)
					blue_go_ahead(i);
				for (int i = 0; i != CWarrior::ci_wa.size(); i++)
					for (int j = 0; j != CWarrior::ci_wa[i].size(); j++)
						CWarrior::ci_wa[i][j]->PrintGoahead(i);
				CWarrior::time.minute = 35;
			}
			else
				break;
			if (CWarrior::time.tran_minu() <= t && !CWarrior::game_over) {//4 theft weapon
				int city_no = 0;
				for (vector<w_c>::iterator i = CWarrior::ci_wa.begin(); i != CWarrior::ci_wa.end(); i++) {
					w_c deq_w = *i;
					if (deq_w.size() == 2)
						ci_theft(deq_w, city_no);
					city_no++;
				}
				CWarrior::time.minute = 40;
			}
			else
				break;
			if (CWarrior::time.tran_minu() <= t) {//4 attack atc
				int city_no = 0;
				for (vector<w_c>::iterator i = CWarrior::ci_wa.begin(); i != CWarrior::ci_wa.end(); i++) {
					w_c& deq_w = *i;//引用可改变
					if (deq_w.size() == 2)
						ci_battle(deq_w, city_no);
					city_no++;
				}
				CWarrior::time.minute = 50;
			}
			else
				break;
			if (CWarrior::time.tran_minu() <= t) {//5 hq report
				cout << CWarrior::time << RedHead.Get_LifeValue() << " elements in red headquarter" << endl;
				cout << CWarrior::time << BlueHead.Get_LifeValue() << " elements in blue headquarter" << endl;
				CWarrior::time.minute = 55;
			}
			if (CWarrior::time.tran_minu() <= t) {//6 wa report
				for (vector<w_c>::iterator i = CWarrior::ci_wa.begin(); i != CWarrior::ci_wa.end(); i++) {
					const w_c& deq_w = *i;
					for (auto i = deq_w.begin(); i != deq_w.end(); i++)
						(*i)->report();
				}
				CWarrior::time.minute = 0;
				CWarrior::time.hour++;
			}
			else
				break;
		}//while
		CWarrior::call_destructor();
	}
	system("pause");
	return 0;
}