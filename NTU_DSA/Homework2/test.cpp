#include <bits/stdc++.h>

using namespace std;

template <class eleType> ostream &operator<< (ostream &os, vector <eleType> &vec) {
    for (auto &i : vec) os << i << " ";
    return os;
}

template <class eleType1, class eleType2> ostream &operator<< (ostream &os, pair<eleType1, eleType2> p) {
    os << p.first << " " << p.second;
    return os;
}

template <class eleType> int black_box(vector<eleType> &vec, int i, int j, int k) {
    if(i > j){
        if(i < k) {
            return i;
        }else{
            if(j > k) return j;
            else return k;
        }
    }else {
        if(j < k) {
            return j;
        }else {
            if(i > k) return i;
            else return k;
        }
    }
}

template <class eleType> pair<int, int> boundary_cake(vector <eleType> &vec, int zuo, int you) {
    int left = zuo;
    int right = zuo + 2;
    int mid = zuo + 1;
    int next = zuo + 3;
    
    while(next <= you) {
        int tmp = black_box(vec, vec[left], vec[mid], vec[right]) ;
        
        if(tmp == vec[left]) {
            left = next++;
        }else if (tmp == vec[mid]) {
            mid = next++;
        }else if (tmp == vec[right]){
            right = next++;
        }
        
    }
    if(right == next - 1) {
        return {left , mid};
    }else if (left == next - 1){
        return {mid, right};
    }else if (mid == next - 1) {
        return {left, right};
    }
}

template <class eleType> void insert(vector <eleType> &vec, eleType num, int left, int right){
    if(vec[left] == black_box(vec, vec[right], vec[left], num)){
        int mid = left;
        if(vec.size() == right - left + 1)vec.resize(vec.size() + 1);
        int tmp = vec[mid];
        vec[mid] = num; 
        for(int i = mid + 1; i <= right + 1; ++i) {
            int tmp2 = vec[i];
            vec[i] = tmp;
            tmp = tmp2;
        }
        return;
    }else  if (vec[right] == black_box(vec, vec[right], vec[left], num)) {
        vec[right + 1] = num;
        return;
    }
    int orig_right = right;
    int mid = 0;
    while(left <= right){
        mid  = (left + right) / 2;
        if(num == black_box(vec, vec[right], vec[mid], num)) {
            left = mid + 1;
        }else if(num == black_box(vec, vec[left], vec[mid], num)){
            right = mid;
        }else {
            break;
        }   
                
    }
    if(vec.size() == right - left + 1)vec.resize(vec.size() + 1);
    int tmp = vec[mid];
    vec[mid] = num; 
    for(int i = mid + 1; i <= orig_right + 1; ++i) {
        int tmp2 = vec[i];
        vec[i] = tmp;
        tmp = tmp2;
    }
}


template <class eleType> void sort(vector<eleType> &vec, int left, int right) {
    if(right - left < 2) return;
    sort(vec, left, (left + right) / 2);
    sort(vec, (left + right) / 2 + 1, right);
    int left_left = left;
    int left_right = (left + right) / 2;
    int right_left = (left + right) / 2 + 1;
    int right_right = right;
    
    while(left_left != left_right && right_left != right_right ) {
        auto tmp = boundary_cake(vec, left, right);
        if(tmp.first == vec[left_left] ) {
            ++left_left;
        }
    }
    while(left_left != left_right) {

    }
}

int main(){
    vector<int> vec = {6, 8, 1, 7, 100, 150, 80};
    sort(vec, 0, vec.size() - 1);
    cout << "Ans: " << vec << endl;
    
}
