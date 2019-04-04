#include<iostream>
#include<string>
#include<set>
using namespace std;
// �õ�next����
void getNext(string ss, int *next)
{
    int j = 0, k = -1;
    next[0] = -1;
    while(j < ss.size() - 1) {
        if(k == -1 || ss[j] == ss[k] || ss[k] == '?') {	// ע�������ʺ�Ҳ������Ҫ���
            j++;
            k++;
            next[j] = k;
        }
        else{
            k = next[k];
        }
    }
}
// kmp�㷨��ע�����Ƿ���s�е�һ�γ���ss��index����Ϊ��������Ҫ����s�����һ�γ���ss��index
int kmp(string s, string ss)	
{
    int len_s = s.size();
    int len_ss = ss.size();
    int i = 0, j = 0;
    int *next = new int[len_ss];
    getNext(ss, next);
    while(i < len_s) {
        if(j == -1 || s[i] == ss[j] ||ss[j] == '?') {	// ͬ��ע�������ʺ�Ҳ����ƥ��Ҫ���
            i++;
            j++;
        }
        else {
            j = next[j];	
        }
        if (j == len_ss) {
		    delete [] next;
            return i - len_ss;
        }
	}
    delete [] next;
    return -1;
}
// ���ˣ����Ŀ�ʼ�c(������)?
bool isMatch(string s, string p) {
    if(p == "*")
        return true; 
    int len_p = p.size();
    set<int> s_p {};
    for(int i = 0; i < len_p - 1; i++)
    {
        if(p[i] == '*')
            s_p.insert(i);
        if(p[i] == '*'&& p[i+1] == '*')
        {
            p.erase(i, 1);
            i--;
            len_p--;	
        }	
    } 
    if(p[len_p-1] == '*')
        s_p.insert(len_p-1);
    auto iter1 = s_p.begin();
    auto iter2 = s_p.end();
    int count = 0; // count *
    for(auto p = iter1; p != iter2; p++)
    {
        count++;
    }
    if(count == 0) {
        if(s == ""&& p =="")
            return true;
        else if(s == ""|| p == "")
            return false;
        else if(s.size() != p.size() || kmp(s, p) != 0) {
            return false;
        }
        else{
            return true;
        }
    }
    int len_s = s.size();
    int len_x = len_s - (len_p - count);	// * ������ܳ��� 
    if(len_x < 0)
        return false;
    else{
        // ������*��index����������
        int *index = new int [count]; 
        int i = 0; 
        for(auto p = iter1; p != iter2; p++)
        {
            index[i] = *p;
            i++;
        } 
        // �� * �� p ���ָ�Ϊ ��ͬ�� �Ӵ� s1, s2, s3���� 
        // �����β���� *
        if(count > 1&& index[0] == 0&& index[count-1] == len_p-1)
        {
            if(count == 2)
            {
                p = p.erase(0, 1);
                p = p.erase(p.size() - 1, 1);
                if(kmp(s, p) != -1) {
                	delete [] index;
					return true;
				}
                else {
                	delete [] index;
                    return false;
                }
            }
            string str {};
            str = p.substr(1, index[1] - 1);
            int index_sub = kmp(s, str);
            if(index_sub == -1) {
            	delete [] index;
            	return false;
			}
            else{
                p = p.erase(0, index[1]);
                s = s.erase(0, index_sub + str.size());
                delete [] index;
				return isMatch(s, p);
            }
        } 
        // ������� *��βû�� * 
        if(index[0] == 0&& index[count-1] != len_p-1) {
            if(count == 1)
            {
                string s1{s.rbegin(), s.rend()};
                p = p.erase(0, 1);
                string p1{p.rbegin(), p.rend()};
                if(kmp(s1, p1) == 0) {
				    delete [] index;
                    return true;
                }
                else {
				    delete [] index;
                    return false;
                }
            }
            else{ // ���������� * 
                string temp = p.substr(index[count-1] + 1, len_p - 1 - index[count-1]);
				// ��ת�ַ���������kmp���������ó��������ͻ����s�����һ�γ���ss��λ��
                string temp1(temp.rbegin(), temp.rend());	
                string s1(s.rbegin(), s.rend());

                int index_last = s.size() - 1 - (kmp(s1, temp1) + temp1.size() - 1);
                if(index_last == -1|| index_last + temp.size() != s.size())
                {
                	delete [] index;
                    return false;
                }
                else{
                    s = s.erase(index_last, temp.size());
                    p = p.erase(index[count-1] + 1, temp.size());
                    delete [] index;
				    return isMatch(s, p);
                }
            }
        }
        // ���β�� *����û�� * 
        if(index[0] != 0&& index[count-1] == len_p-1) {
            if(count == 1)
            {
                if(kmp(s, p.erase(index[count-1], 1)) == 0) {
                	delete [] index;
                    return true;
                }
                else {
                	delete [] index;
                    return false;
                }
            }
            else {
                string temp = p.substr(0, index[0]);
                int index_first = kmp(s, temp);
                if(index_first == -1|| index_first != 0)
                {
                	delete [] index;
                    return false;
                }
                else{
                    s = s.erase(0, temp.size());
                    p = p.erase(0, temp.size());
                    delete [] index;
                    return isMatch(s, p);  
                }
            }
        }
        // �����β��û�� *
        if(index[0] != 0&& index[count-1] != len_p-1)
        {
            string temp1 = p.substr(0, index[0]);
            int index_first = kmp(s, temp1);
            string temp2 = p.substr(index[count-1] + 1, len_p - 1 - index[count-1]);

            string temp3(temp2.rbegin(), temp2.rend()); 
            string s1(s.rbegin(), s.rend());

            int index_last = s.size() - 1 - (kmp(s1, temp3) + temp3.size() - 1);	 
            if(index_last == -1|| index_first == -1|| index_first != 0|| index_last + temp2.size() != s.size())
            {
            	delete [] index;
                return false;
            }
            s = s.erase(0, temp1.size());
            p = p.erase(0, temp1.size());
            s = s.erase(index_last - temp1.size(), temp2.size());
            p = p.erase(index[count-1] + 1 - temp1.size(), temp2.size());
            delete [] index;
            return isMatch(s, p);
        }
    } 
    return true;
}
int main()
{ 
	string s {};
	string p {};
	while(cin>>s>>p) {
		if(s == "-1")	// ������ s Ϊ -1 �˳����� 
			return 0; 
		if(isMatch(s, p)) {
			cout<<"ƥ��"<<endl; 
		}
		else {
			cout<<"��ƥ��"<<endl;
		}
	}
	return 0;
} 
