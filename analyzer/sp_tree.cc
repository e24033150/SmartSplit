#include "sp_tree.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <cmath>
#include <iostream>

split_node::split_node(range rg, int d)
{
    r = rg;
    left = NULL;
    right = NULL;
    cnt = 0;
    r_cnt = 0;
    depth = d;
    btd = -1;
    step = -1;
    p = 0.0;
    h = 0.0;
}

split_node::~split_node()
{
    rl.clear();
}


split_tree::split_tree(range r, int th, int dl):root(r, 1)
{
    tsh = th;
    max_depth = 0x0;
    min_btd = 32;
    depth_sum = 0;
    depth_limit = dl;
    leaf_cnt = 0;
    max_step = 1;
    max_btd = 0;
    
}


split_tree::~split_tree()
{
    for(list<split_node*>::iterator it = nl.begin();
            it != nl.end();
            it ++ ) {
        delete (*it);
    }
}


void split_tree::split_add_range(const range &r, int cnt)
{
    range rb;
    rb.low = max(r.low, root.r.low);
    rb.high = min(r.high, root.r.high);

    root.rl.push_back(pair<range, int>(rb, cnt));
    root.cnt += cnt;
    root.r_cnt += 1;
}

void split_tree::compact_range(split_node &n)
{
    range final;
    final.high = 0;
    final.low = 0xFFFFFFFF;

    for(list<pair<range, int> >::iterator li = n.rl.begin();
            li != n.rl.end();
            ++ li) {
        if(li->first.high > final.high)
            final.high = li->first.high;
        if(li->first.low < final.low)
            final.low = li->first.low;
    }

    n.r = final;
}


void split_tree::split_ranges(split_node *curr_node)
{
    int lflag = 0;
    int rflag = 0;
    for(list<pair<range,int> >::iterator rit = curr_node->rl.begin();
            rit != curr_node->rl.end();
            rit ++ ) {
        lflag = 0;
        rflag = 0;

        if(curr_node->left) {
            if(rit->first.low <= curr_node->left->r.high
                    && rit->first.high >= curr_node->left->r.low)
            {
                //printf("add left\n");
                //curr_node->left->rl.push_back(*rit);
                //curr_node->left->cnt += rit->second;
                //curr_node->left->r_cnt += 1;
                lflag =1;
            }
        }


        if(curr_node->right) {
            if(rit->first.low <= curr_node->right->r.high
                    && rit->first.high >= curr_node->right->r.low)
            {
                //printf("add right\n");
                //curr_node->right->rl.push_back(*rit);
                //curr_node->right->cnt += rit->second;
                //curr_node->right->r_cnt += 1;
                rflag = 1;
                //if(flag ==1) {
                //    //printf("range %llu-%llu copy\n", rit->first.low, rit->first.high);
                //    //printf("range duplicated\n");
                //}
            }
        }

        if (rflag && lflag) {
            continue;
        }
        if (rflag && !lflag) {
            curr_node->right->rl.push_back(*rit);
            curr_node->right->cnt += rit->second;
            curr_node->right->r_cnt += 1;
        }
        if (lflag && !rflag) {
            curr_node->left->rl.push_back(*rit);
            curr_node->left->cnt += rit->second;
            curr_node->left->r_cnt += 1;
        }

    }

    curr_node->rl.clear();
    //curr_node->cnt = 0;
}

void split_tree::node_update(split_node *curr_node)
{
    //if(curr_node->cnt < tsh)
    //    leaf_cnt += 1;

    if(curr_node->left == NULL && curr_node->right == NULL) {
        //leaf
        //if(curr_node->depth > max_depth)
        //    max_depth = curr_node->depth;

        //depth_sum += (double)curr_node->depth * curr_node->cnt /root.cnt;
        //leaf_cnt += (double)curr_node->cnt/root.cnt;
        leaf_cnt += 1;
    }
}
# if 0
int get_balance_tree_depth(split_node *root) 
{
    list<split_node*> snl;
    snl.push_back(root);
    split_node *curr_node;
    int btd = 0;
    int this_level = 1;
    int next_level = 0;


    while(!snl.empty()) {

        for(int i=0; i< this_level; i++) {

            curr_node = snl.front();
            snl.pop_front();
            
            if(curr_node->left) {
                snl.push_back(curr_node->left);
                next_level++;
            }

            if(curr_node->right) {
                snl.push_back(curr_node->right);
                next_level++;
            }
        }

        if((double) next_level/ this_level < 1.5) {
            break;
        }

        btd++;
        this_level = next_level;
        next_level =0;
    }

    snl.clear();
    return btd ;
}
#endif 
split_node * following_the_line(split_node* n)
{
    split_node *fl = n;
    while(!(fl->left) || !(fl->right)) {

        if(!(fl->left) && !(fl->right)) {
            break;
        }
        if((fl->left) && !(fl->right)){
            fl = fl->left;
        }
        else if(!(fl->left) && (fl->right)) {
            fl = fl->right;
        }
        
    }

    return fl;
}

split_node * following_the_line(split_node* n, int & skip)
{
    split_node *fl = n;
    while(!(fl->left) || !(fl->right)) {

        if(!(fl->left) && !(fl->right)) {
            break;
        }
        if((fl->left) && !(fl->right)){
            fl = fl->left;
            skip ++;
        }
        if(!(fl->left) && (fl->right)) {
            fl = fl->right;
            skip ++;
        }
        
    }

    return fl;
}
#if 0
void split_tree::twice_balance_tree_depth(list<split_node*> & leaf, vector<int> &btd_l, int &skip)
{
    for(list<split_node*>::iterator it = leaf.begin();
            it != leaf.end();
            it ++ ) {
        split_node *n = following_the_line(*it, skip);
        int d = get_balance_tree_depth(n);
        btd_l.push_back(d);
    }
}
#endif

int balance_tree_depth(split_node *root, list<split_node*> & leaf)
{
    list<split_node*> snl;
    snl.push_back(root);
    split_node *curr_node;
    int btd = 0;
    int this_level = 1;
    int next_level = 0;


    while(!snl.empty()) {

        for(int i=0; i< this_level; i++) {

            curr_node = snl.front();
            snl.pop_front();
            
            if(curr_node->left) {
                snl.push_back(curr_node->left);
                leaf.push_back(curr_node->left);
                next_level++;
            }

            if(curr_node->right) {
                snl.push_back(curr_node->right);
                leaf.push_back(curr_node->right);
                next_level++;
            }
        }

        if((double) next_level/ this_level < 1.5) {
//actually set to 2 is also ok
            break;
        }

        btd++;
        this_level = next_level;
        next_level =0;
        leaf.clear();
    }

    snl.clear();
    return btd ;

}

int split_tree::get_bd()
{
    list<split_node*> leaf;
    return balance_tree_depth(&root, leaf);
}

void dump_tree_node(split_node* n, string file) 
{
    list<split_node*> snl;
    snl.push_back(n);
    split_node *curr_node;
    int node = 0;
    int av_node = 1;

    FILE *fp = fopen(file.c_str(), "w");



    while(!snl.empty())
    {
        curr_node = snl.front();
        snl.pop_front();

        fprintf(fp, "node %d\n", node);

        if(curr_node->left) {
            fprintf(fp, "\tlchild %d\n", av_node);
            snl.push_back(curr_node->left);
            av_node++;
        }

        if(curr_node->right) {
            fprintf(fp, "\trchild %d\n", av_node);
            snl.push_back(curr_node->right);
            av_node++;
        }
        fprintf(fp, "\tcnt %d\n", curr_node->cnt);


        node ++;

    }

    fclose(fp);


}


void split_tree::measuring_hk(vector<double> &hk) 
{
    list<split_node*> snl;
    snl.push_back(&root);
    split_node *curr_node;

    int this_level = 1;
    int next_level = 0;
    double dhk = 0;
    double sump = 0;

    cout<<"hfinal "<<measure_h(&root)<<endl;
    cout<<"sump "<<root.p<<endl;

    update_p(&root, root.p);

    while(!snl.empty()) {

        dhk = 0;
        sump = 0;
        for(int i=0; i< this_level; i++) {

            curr_node = snl.front();
            snl.pop_front();
            
            if(curr_node->left) {
                snl.push_back(curr_node->left);
                next_level++;
            }

            if(curr_node->right) {
                snl.push_back(curr_node->right);
                next_level++;
            }

            //p 
            dhk += curr_node->h;
            sump += curr_node->p;
            //p/sump

            //dhk += (curr_node->h - curr_node->p * log(root.p)/log(2))/root.p;
            //sump += curr_node->p/root.p;
            
        }
        if(1-sump >=1e-9 || 1-sump <=-(1e-9))
            dhk += (1-sump) * log(1-sump)/log(2);
        hk.push_back(dhk);

        this_level = next_level;
        next_level =0;
    }

}

void split_tree::update_p(split_node *n, double sum_p)
{
    n->p = n->p/sum_p;
    n->h = n->p * log(n->p)/log(2);
    
    if(n->left)
        update_p(n->left, sum_p);
    if(n->right)
        update_p(n->right, sum_p);
}

double split_tree::measure_h(split_node* n) 
{
    double ret = 0;

    if(!n->left && !n->right) {
        double p;
        p = (double)(n->r.high - n->r.low +1)/(root.r.high - root.r.low +1);
        ret = p * log(p)/log(2);  
        n->p = p;
        n->h = ret;
        return ret;
    }

    if(n->left){
        ret = measure_h(n->left);
        n->h += ret;
        n->p += n->left->p;
    }
    if(n->right){
        ret = measure_h(n->right);
        n->h += ret;
        n->p += n->right->p;
    }

    return n->h;
}

void split_tree::measure_steps(split_node* n, int step, int label)
{
    list<split_node*> leaf;
    int btd = balance_tree_depth(n, leaf);
    if(btd > max_btd)
        max_btd = btd;

    if(label) {
        n->step = step;
        n->btd = btd;
    }
    //if(btd == 11){
    //    printf("n->cnt %d\n", n->cnt);
    //}
    //if(step == 3) {
    //    printf("n->cnt %d, btd %d\n", n->cnt, btd);
    //}


    //printf("btd %d\n", btd);

    if(step > max_step)
        max_step = step;
    

    for(list<split_node*>::iterator it = leaf.begin();
            it != leaf.end();
            it ++ ) {

        split_node * next = following_the_line(*it);
        //split_node * next = *it; 
        if((!(next->left)) && (!(next->right))) {
            continue;
        }
        //if(!(n->left) && !(n->right))
        //    continue;
        if(next->cnt < tsh)
            continue;

        //if(step == 5) {
        //    dump_tree_node(n, "b");
        //    exit(-1);
        //}

        measure_steps(next, step + 1, label);
    }
}

int split_tree::measuring_steps(int &btd)
{
    split_node * n = following_the_line(&root);
    measure_steps(n, 1, 1);
    btd = max_btd;
    return max_step;
}

void split_tree::find_tail(split_node *n, vector<range> &ht, int allow_step)
{
    int old_max_btd = max_btd;
    int old_max_step = max_step;

    max_btd = 0;
    max_step = 0;

    list<split_node*> leaf;
    balance_tree_depth(n, leaf);

    for(list<split_node*>::iterator it = leaf.begin();
            it != leaf.end();
            it ++ ) {

        split_node * next = following_the_line(*it);
        if((!(next->left)) && (!(next->right))) {
            continue;
        }
        if(next->cnt < tsh)
            continue;


        measure_steps(next, 1, 0);

        if(max_step > allow_step){
            ht.push_back((*it)->r);
        }
        max_step = 0;
        max_btd = 0;
    }
    max_step = old_max_step;
    max_btd = old_max_btd;

}

void find_maxcnt_leaf(split_node* n, range &r)
{
    list<split_node*> leaf;
    balance_tree_depth(n, leaf);
    int max = 0;
    if(leaf.size() == 0) {
        r = n->r;
        max = n->cnt;
    }
    for(list<split_node*>::iterator it = leaf.begin();
            it != leaf.end();
            it ++ ){
        if((*it)->cnt > max){
            max = (*it)->cnt;
            r = (*it)->r;
        }
        printf("range: %llu - %llu ", (*it)->r.low, (*it)->r.high);
        printf("count %d ", (*it)->cnt);
    }
    printf("max %d\n", max);
}

void split_tree::get_max_skew_range(range &r)
{
    find_maxcnt_leaf(&root, r);
}

void split_tree::get_xbtd_range(vector<vector<range> > &vr, int btd, int allow_step)
{
    list<split_node*> snl;
    snl.push_back(&root);
    split_node *curr_node;
    vector<range> ht;

    while(!snl.empty()) {
        curr_node = snl.front();
        snl.pop_front();

        if(curr_node->btd == btd) {
            ht.push_back(curr_node->r);
            find_tail(curr_node, ht, allow_step);
            vr.push_back(ht);  
            ht.clear();
        }

        if(curr_node->left) {
            snl.push_back(curr_node->left);
        }

        if(curr_node->right) {
            snl.push_back(curr_node->right);
        }
    }
    snl.clear();
}



static inline range range_in_boundary(range r, range boundary)
{
    range ret;
    if (r.low > boundary.low) {
        ret.low = r.low; 
    }
    else {
        ret.low = boundary.low;
    }

    if (r.high < boundary.high) {
        ret.high = r.high;
    }
    else {
        ret.high = boundary.high;
    }
    return ret;
}

int split_tree::small_parts(split_node *n, unsigned long long s)
{
    range ret;
    unsigned long long small = 0xFFFFFFFFULL;
    for(list<pair<range,int> >::iterator it = n->rl.begin();
            it != n->rl.end();
            it ++) {
        ret = range_in_boundary(it->first, n->r);
        if(ret.low < small)
            small = ret.low;
    }

    if(small == 0xFFFFFFFFULL) {
        printf("no ranges inside the node\n");
        return -1;
    }
    //put the range in the right part of the fuse array
    // fuse_array[0] --- fuse_array[1] ---
    // |<------------->|<--->|
    //                   x <---- here is the range lies in.
    //however,...
    if ( (small - root.r.low)/s == 0)
    {
        printf("small: there does not exist a good s for this, I give up\n");
        return -1;
    }

    return (small - root.r.low)/s;

}

int split_tree::large_parts(split_node *n, unsigned long long s)
{
    range ret;
    unsigned long long large = 0x0;
    for(list<pair<range,int> >::iterator it = n->rl.begin();
            it != n->rl.end();
            it ++ ) {
        ret = range_in_boundary(it->first, n->r);
        if(ret.high > large)
            large = ret.high;
    }

    if(large == 0) {
        printf("no ranges inside the node\n");
        return -1;
        //exit(-1);
    }

    if((large - root.r.low)/s == ceil((double)(large - root.r.low)/s)) {
        //when caculating the range, we use the range [root.r.low, root.r.low +fuse * s -1]
        //so if (large - root.r.low) % s == 0
        //then we will have a range [root.r.low, large -1 ]
        //a rule [root.r.low, large] will be in the left part.
        // |<-->|<-------------->|
        //   x<-------- here is the range fall
        // however
        if ( large + s - 1 >= root.r.high) {
            printf("large: there does not exist a good s for this, I give up\n");
            return -1;
        }

        return (large - root.r.low)/s + 1;
    }
    
    return ceil((double)(large - root.r.low)/s);
}

int split_tree::calc_index(split_node *n, unsigned long long s)
{
    //unsigned long long high = 0;
    //unsigned long long high_in_b;
    int index;
    //for(list<pair<range,int> >::iterator rit = n->rl.begin();
    //        rit != n->rl.end();
    //        rit ++ ) {

    //    high_in_b = (rit->first.high > n->r.high) ? n->r.high : rit->first.high; 

    //    if(high_in_b > high)
    //        high = high_in_b;

    //}
    if(s == 0) {
        printf("s==0, bugs\n");
        exit(-1);
    }

    index = ceil((double)(n->r.low - root.r.low) / s);
    return index;

}
//struct sp_pos{
//    split_node *n;
//    int pos;
//};
//

bool comp(split_node *first, split_node *second)
{
    if(first->r.high < second->r.low) {
        return true;
    }
    return false;
    
}


void split_tree::balance_leaves(list<split_node *> &leaves)
{
    int r_cnt = 0;
    for(list<split_node*>::iterator it = leaves.begin();
            it != leaves.end();
            it ++ ) {
        //printf("low %llu high %llu r_cnt %d \n", (*it)->r.low, (*it)->r.high, (*it)->r_cnt);
        r_cnt += (*it)->r_cnt;
    }

    int piece = r_cnt / NUM_INDEX;
    int curr_cnt = 0;
    list<split_node*> choose;
    list<split_node*>::iterator it = leaves.begin();

    while(choose.size() < NUM_INDEX -1 
            && it != leaves.end()) {

        if(curr_cnt < piece) {
            curr_cnt += (*it)->r_cnt;
        }
        else {
            choose.push_back(*it);
            curr_cnt = 0;
        }

        it ++;
    }
    if(choose.size() == 0) 
        choose.push_back(leaves.back());
    leaves.clear();
    leaves = choose;

}

void split_tree::interval_leaves_v(list<split_node*> & leaves, 
        vector<int> &v, unsigned long long s) 
{
    unsigned long long low = root.r.low;
    int next_v;
    v.push_back(0);
    int i = 0;


    for(list<split_node*>::iterator it = leaves.begin();
            it != leaves.end();
            it ++ ) {

        if((*it)->r.low == low) {
            continue;
        }

        next_v = ((*it)->r.low - low) / s;
        if(next_v != v[i]) {
            //printf("low %llu s %llu next_v %d\n", (*it)->r.low, s, next_v); 
            v.push_back(next_v);
            i++;
        }
    }

    if(v.size() == 1) {
        v.push_back(1);
    }
}

void split_tree::merge_leaves_v(list<split_node*> &leaves, vector<int> &v,
        unsigned long long s)
{
    //printf("merged\n");
    int r_cnt = 0;
    for(list<split_node*>::iterator it = leaves.begin();
            it != leaves.end();
            it ++ ) {
        //printf("low %llu high %llu r_cnt %d \n", (*it)->r.low, (*it)->r.high, (*it)->r_cnt);
        r_cnt += (*it)->r_cnt;
    }

    int piece = r_cnt / NUM_INDEX;
    int curr_cnt = 0;
    list<split_node*> choose;
    list<split_node*>::iterator it = leaves.begin();

    while(choose.size() < NUM_INDEX -1 
            && it != leaves.end()) {

        if(curr_cnt < piece) {
            curr_cnt += (*it)->r_cnt;
        }
        else {
            choose.push_back(*it);
            curr_cnt = 0;
        }

        it ++;
    }
    if(choose.size() == 0) 
        choose.push_back(leaves.back());
    interval_leaves_v(choose, v, s);
}

int split_tree::unbalance_tree_depth(int *fuse_array, int *fuse_array_num)
{
    //need to find 7 leaves.
    list<split_node *> leaves;
    list<split_node *> snl;

    //sp_pos root_pos;
    //root_pos.n = &root;
    //root_pos.pos = 0;

    //snl.push_back(root_pos);
    //sp_pos curr_sppos;
    //sp_pos new_sppos;
    
    
    int max_depth = 0;
    snl.push_back(&root);
    split_node * curr_node;
    

    while(!snl.empty())
    {
        curr_node = snl.front();
        snl.pop_front();

        if(curr_node->left == NULL
                && curr_node->right == NULL) {

            leaves.push_back(curr_node);

            //if(leaves.size() == NUM_INDEX - 1)
            //    break;
        }

        if(curr_node->left) {
            //new_sppos.n = curr_sppos.n->left;
            //new_sppos.pos = curr_sppos.pos << 1;
            snl.push_back(curr_node->left);
        }

        if(curr_node->right) {
            //new_sppos.n = curr_sppos.n->right;
            //new_sppos.pos = (curr_sppos.pos << 1) + 1;
            snl.push_back(curr_node->right);
        }
    }

    for(list<split_node*>::iterator it = leaves.begin();
            it != leaves.end(); it++) {
        if ((*it)->depth > max_depth) {
            max_depth = (*it)->depth;
        }
    }


    if(no_div) {
        int i = 0;
        int cuts = 1 << (max_depth - 1);
        unsigned long long s = ceil((double)(root.r.high - root.r.low + 1) / cuts); 

        if(s != (1ULL<<((unsigned)log2(s)))) {
            s = 1ULL<<((unsigned)ceil(log2(s)));
        }

        vector<int> v;
        leaves.sort(comp);

        if(leaves.size() > NUM_INDEX -1 ) { 
            //printf("leaves size %u\n", leaves.size());
            //leaves.resize(NUM_INDEX - 1); 
            //printf("leaves size %u\n", leaves.size());
            merge_leaves_v(leaves, v,s );
        }
        else {
            interval_leaves_v(leaves, v, s);
        }

        for(i = 0; i< (int)v.size(); i++) {
            fuse_array[i] = v[i];
            //printf("fuse_array[%d] %d\n", i, v[i]);
        }

        *fuse_array_num = v.size();
    }
    else {
        if(leaves.size() > NUM_INDEX -1 ) { 
            leaves.sort(comp);

            //printf("leaves size %u\n", leaves.size());
            //leaves.resize(NUM_INDEX - 1); 
            //printf("leaves size %u\n", leaves.size());
            balance_leaves(leaves);

        }


        //
        int i = 0;
        int cuts = 1 << (max_depth - 1);
        unsigned long long s = ceil((double)(root.r.high - root.r.low + 1) / cuts); 
        vector<int> v;

        for(list<split_node *>::iterator it = leaves.begin();
                it != leaves.end(); it++) {
            //int stars = max_depth - it->n->depth;
            //int allones = (1<<stars) - 1;
            //it->pos <<= stars;
            //it->pos += allones;
            //int end = it->pos + 1;
            int index = calc_index(*it, s);

            //if(index > cuts) {
            //    index = cuts;
            //}


            v.push_back(index);
            i++;
        }
        sort(v.begin(), v.end());

        int j;
        int start = 0;
        int fuse_num = 0;


        if((unsigned)v[0] != 0) {
            fuse_array[0] = 0;
            start = 1;
            fuse_num ++;
        }



        for(j=0;j<i;j++) {
            fuse_array[start+j] = v[j];
            fuse_num ++;
            //printf("fuse array %d\n", v[j]);
        }

        //if((unsigned)v[j-1] != cuts) {
        //    fuse_array[j] = cuts;
        //    printf("fuse array %d\n", fuse_array[j]);
        //    i++;
        //}

        //if ((unsigned)v[j-1] != ceil(((double)(root.r.high - root.r.low +1 ))/s)) { 
        //    fuse_array[j] = ceil(((double)(root.r.high - root.r.low + 1))/s); 
        //printf("fuse array %d\n", fuse_array[j]);
        //    i++;
        //}

        if(fuse_num == 1) {
            //this is a very special case,
            //
            //which is that there is only one node, and this node maybe 
            //the root node, or the left most node. 
            //
            //we have to make sure the whole space is divided into 2 parts
            //one part holds all the large range
            //one part holds all the small range
            split_node *n = leaves.front();
            fuse_array[0] = 0; 
            fuse_array[1] = large_parts(n, s);
            fuse_num++;

            if(fuse_array[1] == -1)
            {
                fuse_array[1] = 0;
                fuse_num = 0;
                max_depth = 1;
            }
            //printf("speci add fuse array %d\n", fuse_array[1]);
        }
        //here we actually are doing the work simliar like split_tree::large_parts
        //however, we are tunning the low part, so maybe this can be used as a 
        //small_part.
        else if(fuse_num == 2 && leaves.size() == 1) {
            //only one node
            //it might lied in the right most leaf
            //or it might be one leaf
            //it should not be the root node
            split_node *n = leaves.front();
            fuse_array[1] = small_parts(n, s);
            if(fuse_array[1] == -1) {
                fuse_array[1] = 0;
                fuse_num = 0;
                max_depth = 1;
            }

        }




        //if ((unsigned)v[j-1] != ceil(((double)(root.r.high - root.r.low +1 ))/s)) { 
        //    fuse_array[j] = ceil(((double)(root.r.high - root.r.low + 1))/s); 
        //    printf("fuse array %d\n", fuse_array[j]);
        //    i++;
        //}

        //if(i==1) {
        //    fuse_array[0] = ceil(((double)(root.r.high - root.r.low + 1))/s) -1;
        //    fuse_array[1] = ceil(((double)(root.r.high - root.r.low + 1))/s);
        //    i++;
        //}


        *fuse_array_num = fuse_num;
    }
    //printf("fuse_array num %d\n", *fuse_array_num);
    return max_depth - 1;
}

int split_tree::duplicate_cut(split_node * curr_node)
{

    range r[2];

    if(curr_node->r.low == curr_node->r.high)
        return 1;

    r[0].low = curr_node->r.low;
    r[1].high = curr_node->r.high;

    r[0].high = (curr_node->r.high - curr_node->r.low + 1)/2 + curr_node->r.low - 1;
    r[1].low = r[0].high +1;

    int dup;
    int total_dup = 0;

    for(list<pair<range,int> >::iterator rit = curr_node->rl.begin();
            rit != curr_node->rl.end();
            rit ++ ) {
        dup = 0;

        if(rit->first.low <= r[0].high
                    && rit->first.high >= r[0].low) {
            dup ++;
        }

        if(rit->first.low <= r[1].high
                && rit->first.high >= r[1].low) {
            dup ++;
        }

        if(dup == 2) {
            total_dup ++;

        }
    }

    if( (size_t)total_dup == curr_node->rl.size()) {
        //printf("dupliated!\n");
        return 1;
    }

    return 0;

}

split_node *split_tree::choose_a_suitable_leaf(list<split_node*> &snl)
{

    range r[2];
    int sprl_cnt[2] = {0, 0};
    int orignal = 0;

    double fac;
    double minfac = 0xffffffff;
    split_node *minn = NULL;

    for(list<split_node*>::iterator it = snl.begin();
            it != snl.end();
            it ++) {

        if((*it)->r.low == (*it)->r.high) {
            continue;
        }

        r[0].low = (*it)->r.low;
        r[1].high = (*it)->r.high;

        r[0].high = ((*it)->r.high - (*it)->r.low + 1)/2 + (*it)->r.low - 1;
        r[1].low = r[0].high +1;

        sprl_cnt[0] = 0;
        sprl_cnt[1] = 0;
        orignal = (*it)->r_cnt;
        

        for(list<pair<range,int> >::iterator rit = (*it)->rl.begin();
            rit != (*it)->rl.end();
            rit ++ ) {

            if(rit->first.low <= r[0].high
                    && rit->first.high >= r[0].low)
            {
                sprl_cnt[0] += rit->second;
            }

            if(rit->first.low <= r[1].high
                    && rit->first.high >= r[1].low)
            {
                sprl_cnt[1] += rit->second;
            }
    
        }
        
        fac = sprl_cnt[0] * sprl_cnt[0] + sprl_cnt[1] * sprl_cnt[1]
            - orignal * orignal; 

        if ( fac < minfac) {
            fac = minfac;
            minn = *it;
        }
    }

    return minn;

}

void split_tree::split_create_evenness()
{
    list<split_node*> snl;
    split_node *curr_node;

    range r[2];
    int leaves_cnt = 0;

    snl.push_back(&root);
    leaves_cnt = 1;


    while(leaves_cnt < NUM_INDEX - 1) {
        curr_node = choose_a_suitable_leaf(snl);

        if(curr_node == NULL) 
            break;
#if 0
        if(duplicate_cut(curr_node))
            break;
#endif

        snl.remove(curr_node);

        r[0].low = curr_node->r.low;
        r[1].high = curr_node->r.high;

        r[0].high = (curr_node->r.high - curr_node->r.low + 1)/2 + curr_node->r.low - 1;
        r[1].low = r[0].high +1;

        if(r[0].low <= r[0].high){
            //printf("new node %d left\n", curr_node->depth + 1);
            curr_node->left = new split_node(r[0], curr_node->depth + 1);
        }


        if(r[1].low <= r[1].high) {
            //printf("new node %d right\n", curr_node->depth + 1);
            curr_node->right = new split_node(r[1], curr_node->depth + 1);
        }

        split_ranges(curr_node);
        leaves_cnt --;

        if(curr_node->left != NULL){
            snl.push_back(curr_node->left);
            nl.push_back(curr_node->left);
            leaves_cnt ++;
        }

        if(curr_node->right != NULL) {
            snl.push_back(curr_node->right);
            nl.push_back(curr_node->right);
            leaves_cnt ++;
        }
    }

}

int split_tree::need_delete_node(split_node *node)
{
    range rg;
    size_t cnt = 0;
    for(list<pair<range, int> >::iterator it = node->rl.begin();
            it != node->rl.end();
            it ++ ) {
        rg = range_in_boundary(it->first, node->r);
        if(rg.low == node->r.low && rg.high == node->r.high) {
            cnt ++;
        }
    }
    if(cnt == node->rl.size()) {
        return 1;
    }
    else
        return 0;
    
}

void split_tree::split_create_cost()
{
    list<split_node*> snl;
    split_node *curr_node;
    
    range r[2];
    //int leaves_cnt = 0;

    //compact_range();

    snl.push_back(&root);
    //int left_cnt;
    //int right_cnt;

    while(!snl.empty())
    {
        curr_node = snl.front();
        //printf("snl.size() %lu\n", snl.size());
        snl.pop_front();


        if(curr_node->cnt < tsh || curr_node->depth > depth_limit) { 
            node_update(curr_node);
            continue;
        }

        r[0].low = curr_node->r.low;
        r[1].high = curr_node->r.high;

        r[0].high = (curr_node->r.high - curr_node->r.low + 1)/2 + curr_node->r.low - 1;
        r[1].low = r[0].high +1;

        if(r[0].low <= r[0].high){
            //printf("new node %d left\n", curr_node->depth + 1);
            curr_node->left = new split_node(r[0], curr_node->depth + 1);
        }


        if(r[1].low <= r[1].high) {
            //printf("new node %d right\n", curr_node->depth + 1);
            curr_node->right = new split_node(r[1], curr_node->depth + 1);
        }

        split_ranges(curr_node);
        //left_cnt = curr_node->left->cnt;
        //right_cnt = curr_node->right->cnt;



        if((curr_node->left && curr_node->left->cnt == 0) || need_delete_node(curr_node->left)) {
            delete curr_node->left;
            curr_node->left = NULL;
        }

        if((curr_node->right && curr_node->right->cnt == 0) || need_delete_node(curr_node->right)) {
            delete curr_node->right;
            curr_node->right = NULL;
        }


        if(curr_node->left != NULL){
            snl.push_back(curr_node->left);
            nl.push_back(curr_node->left);
        }

        if(curr_node->right != NULL) {
            snl.push_back(curr_node->right);
            nl.push_back(curr_node->right);
        }

        //node_update(curr_node);

    }

}
void split_tree::split_create_breadth()
{
    list<split_node*> snl;
    split_node *curr_node;
    
    range r[2];
    int leaves_cnt = 0;

    //compact_range();

    snl.push_back(&root);
    //int left_cnt;
    //int right_cnt;

    while(!snl.empty())
    {
        curr_node = snl.front();
        //printf("snl.size() %lu\n", snl.size());
        snl.pop_front();


        if(duplicate_cut(curr_node) || curr_node->depth > depth_limit) { 
            leaves_cnt ++;
            if(leaves_cnt > NUM_INDEX -1 )
                break;
            else 
                continue;
        }

        r[0].low = curr_node->r.low;
        r[1].high = curr_node->r.high;

        r[0].high = (curr_node->r.high - curr_node->r.low + 1)/2 + curr_node->r.low - 1;
        r[1].low = r[0].high +1;

        if(r[0].low <= r[0].high){
            //printf("new node %d left\n", curr_node->depth + 1);
            curr_node->left = new split_node(r[0], curr_node->depth + 1);
        }


        if(r[1].low <= r[1].high) {
            //printf("new node %d right\n", curr_node->depth + 1);
            curr_node->right = new split_node(r[1], curr_node->depth + 1);
        }

        split_ranges(curr_node);
        //left_cnt = curr_node->left->cnt;
        //right_cnt = curr_node->right->cnt;



        if(curr_node->left && curr_node->left->cnt == 0) {
            delete curr_node->left;
            curr_node->left = NULL;
        }

        if(curr_node->right && curr_node->right->cnt == 0) {
            delete curr_node->right;
            curr_node->right = NULL;
        }


        if(curr_node->left != NULL){
            snl.push_back(curr_node->left);
            nl.push_back(curr_node->left);
        }

        if(curr_node->right != NULL) {
            snl.push_back(curr_node->right);
            nl.push_back(curr_node->right);
        }

        //node_update(curr_node);

    }

}


void split_tree::split_create()
{
    list<split_node*> snl;
    split_node *curr_node;
    
    range r[2];

    //compact_range();

    snl.push_back(&root);
    //int left_cnt;
    //int right_cnt;

    while(!snl.empty())
    {
        curr_node = snl.front();
        //printf("snl.size() %lu\n", snl.size());
        snl.pop_front();

        //compact_range(*curr_node);

        if(duplicate_cut(curr_node) || 
                curr_node->depth > depth_limit
                 || curr_node->cnt <= tsh
                ) { 
            node_update(curr_node);
            continue;
        }


        
        r[0].low = curr_node->r.low;
        r[1].high = curr_node->r.high;

        r[0].high = (curr_node->r.high - curr_node->r.low + 1)/2 + curr_node->r.low - 1;
        r[1].low = r[0].high +1;

        if(r[0].low <= r[0].high){
            //printf("new node %d left\n", curr_node->depth + 1);
            curr_node->left = new split_node(r[0], curr_node->depth + 1);
        }


        if(r[1].low <= r[1].high) {
            //printf("new node %d right\n", curr_node->depth + 1);
            curr_node->right = new split_node(r[1], curr_node->depth + 1);
        }

        split_ranges(curr_node);
        //left_cnt = curr_node->left->cnt;
        //right_cnt = curr_node->right->cnt;



        if(curr_node->left && curr_node->left->cnt == 0) {
            delete curr_node->left;
            curr_node->left = NULL;
        }

        if(curr_node->right && curr_node->right->cnt == 0) {
            delete curr_node->right;
            curr_node->right = NULL;
        }


        if(curr_node->left != NULL){
            snl.push_back(curr_node->left);
            nl.push_back(curr_node->left);
        }

        if(curr_node->right != NULL) {
            snl.push_back(curr_node->right);
            nl.push_back(curr_node->right);
        }

        node_update(curr_node);

    }

}

int split_tree::leaf_num()
{
    return leaf_cnt;
}

int split_tree::height()
{
    printf("leaf cnt %d\n", leaf_cnt);
    return max_depth;
}

double split_tree::ave_height()
{
    return (double)depth_sum/leaf_cnt;
} 


void split_tree::dump_tree_with_level(string str, int level)
{
    list<split_node*> snl;
    snl.push_back(&root);
    split_node *curr_node;
    int node = 0;
    int av_node = 1;

    FILE *fp = fopen(str.c_str(), "w");



    while(!snl.empty())
    {
        curr_node = snl.front();
        snl.pop_front();
        if(curr_node->depth > level)
            continue;

        fprintf(fp, "node %d\n", node);

        if(curr_node->left) {
            if(curr_node->depth < level){
                fprintf(fp, "\tlchild %d\n", av_node);
                av_node++;
            }
            snl.push_back(curr_node->left);
        }

        if(curr_node->right) {
            if(curr_node->depth < level){
                fprintf(fp, "\tlchild %d\n", av_node);
                av_node++;
            }
            snl.push_back(curr_node->right);
        }


        fprintf(fp, "\tcnt %d\n", curr_node->r_cnt);


        node ++;

    }

    fclose(fp);

}


void split_tree::dump_tree(string file)
{
    list<split_node*> snl;
    snl.push_back(&root);
    split_node *curr_node;
    int node = 0;
    int av_node = 1;

    FILE *fp = fopen(file.c_str(), "w");



    while(!snl.empty())
    {
        curr_node = snl.front();
        snl.pop_front();

        fprintf(fp, "node %d\n", node);

        if(curr_node->left) {
            fprintf(fp, "\tlchild %d\n", av_node);
            snl.push_back(curr_node->left);
            av_node++;
        }

        if(curr_node->right) {
            fprintf(fp, "\trchild %d\n", av_node);
            snl.push_back(curr_node->right);
            av_node++;
        }


        fprintf(fp, "\tcnt %d\n", curr_node->cnt);


        node ++;

    }

    fclose(fp);

}













