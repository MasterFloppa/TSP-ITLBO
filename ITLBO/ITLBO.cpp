#include <bits/stdc++.h>
using namespace std;

#define mod 1000000007
#define ll long long int

double opt_func(vector<double> values)
{
    int res = 0;
    // res = 100 * (values[0] * values[0] - values[1] * values[1]) + (1 - values[0] * values[0]);
    res = values[0] * values[0] - values[1] * values[1];
    return res;
}

int find_best_teacher(vector<vector<double>> values, int opt)
{
    double maxi = -1e9;
    double mini = 1e9;
    int res_ind = 0;
    for (int i = 0; i < values.size(); i++)
    {
        double res = opt_func(values[i]);
        if (res > maxi && opt == 1)
        {
            maxi = res;
            res_ind = i;
        }
        if (res < mini && opt == 0)
        {
            mini = res;
            res_ind = i;
        }
    }
    return res_ind;
}

vector<int> find_other_teacher(vector<vector<double>> values, int chief_teacher, int opt, int lb, int ub, int t)
{
    // Calculate the fitness value of the chief teacher's solution
    double chief_res = opt_func(values[chief_teacher]);

    vector<int> teacher;
    vector<int> visited(values.size(), 0);
    visited[chief_teacher] = 1;
    cout << "Chief res: " << chief_res << endl;
    cout << "Chief teacher: " << chief_teacher << endl;
    for (int i = 0; i < t; i++)
    {
        double rand_num = (rand() % (int(chief_res * 100) + 100)) + int(chief_res * 100); // took lower bound as chief_res*100 and upper bound as 100
        double val = rand_num / 100.00 - chief_res;
        double mini = 1e9;
        int index = -1;
        for (int j = 0; j < values.size(); j++)
        {
            if (visited[j] == 0 && mini > abs(opt_func(values[j]) - val))
            {
                mini = abs(opt_func(values[j]) - val);
                index = j;
            }
        }
        cout << "Index: " << index << endl;
        visited[index] = 1;
        teacher.push_back(index);
    }

    cout << "Teacher size: " << teacher.size() << endl;
    return teacher;
}

int main()
{
    // Step 1
    int n;
    cout << "Enter the number of students ";
    cin >> n;
    int m;
    cout << "Enter the number of subjects ";
    cin >> m;
    int opt;
    cout << "Select the type of optimization problem 1.Maximization 0.Minimization";
    cin >> opt;

    double lb, ub;
    cout << "Enter the lower bound ";
    cin >> lb;
    cout << "Enter the upper bound ";
    cin >> ub;

    // Step 2
    vector<vector<double>> values(n, vector<double>(m, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> values[i][j];
        }
    }

    // old array
    vector<vector<double>> old_values = values;
    int T = 5;
    while (T--)
    {
        // Step 3
        cout << "Step 3" << endl;
        int chief_teacher = find_best_teacher(values, opt);
        // cout << chief_teacher << endl; // stores the index of the row of the chief teacher

        // Step4
        cout << "Step 4" << endl;
        // No mention on how to select the value of number of teachers
        n = values.size();
        int t = n / 4;
        vector<int> other_teacher = find_other_teacher(values, chief_teacher, opt, lb, ub, t - 1);

        // Sort other teachers based on the fitness value increasing order
        sort(other_teacher.begin(), other_teacher.end(), [&](int a, int b)
             { return opt_func(values[a]) < opt_func(values[b]); });

        t = other_teacher.size() + 1;

        // Step 5
        cout << "Step 5" << endl;
        unordered_map<int, vector<vector<double>>> groups; // map each teacher to group of students
        unordered_map<int, int> stud_grp;                  // map each student to group of teacher
        unordered_map<int, vector<int>> teach_grp;
        if (opt == 1)
        {
            for (int i = 0; i < t - 1; i++) // itterating over all other teachers
            {
                double val1, val2;
                if (i == 0) // for chief teacher
                {
                    val1 = opt_func(values[chief_teacher]);
                    val2 = opt_func(values[other_teacher[0]]);
                }
                else
                {
                    val1 = opt_func(values[other_teacher[i - 1]]);
                    val2 = opt_func(values[other_teacher[i]]);
                }

                for (int j = 0; j < n; j++) // itterating over all students (teacher are also in this list)
                {
                    double val = opt_func(values[j]);
                    if (val <= val1 && val > val2)
                    {
                        if (i == 0)
                        {
                            groups[chief_teacher].push_back(values[j]); // add that student to the group of the chief teacher
                            stud_grp[j] = 0;                            // map that student to the group of the chief teacher
                        }
                        else
                        {
                            groups[other_teacher[i - 1]].push_back(values[j]);
                            stud_grp[j] = i;
                        }
                    }
                }
            }
            // other_teacher[t-2] has the last teacher index stored
            double last_val = opt_func(values[other_teacher[t - 2]]);
            for (int j = 0; j < n; j++) // itterating over all students
            {
                double val = opt_func(values[j]);
                if (val <= last_val) // all students with fitness value less than or equal to last_val are assigned to the last teacher
                {
                    groups[other_teacher[t - 2]].push_back(values[j]);
                    stud_grp[j] = t - 1;
                }
            }
        }
        else
        {
            // iterate over students and assign them to teacher, by checking the the ranges in which the fitness value lies
            for (int i = 0; i < n; i++) // ittearate over all students
            {
                double val = opt_func(values[i]); // get the fitness value of the student
                // check the range in which the fitness value lies
                if (chief_teacher == i || (val >= opt_func(values[chief_teacher]) && (t == 1 || val < opt_func(values[other_teacher[0]]))))
                {
                    groups[chief_teacher].push_back(values[i]);
                    stud_grp[i] = 0; // index of the teacher in the teacher array
                }
                else
                {
                    int flag = 0;
                    for (int j = 1; j < t - 1; j++) // iterate over all other teachers
                    {
                        if (i == other_teacher[j - 1] || (val >= opt_func(values[other_teacher[j - 1]]) && val < opt_func(values[other_teacher[j]])))
                        {
                            groups[other_teacher[j - 1]].push_back(values[i]);
                            stud_grp[i] = j;
                            flag = 1;
                            // cout << i << " " << other_teacher[j - 1] << " " << opt_func(values[other_teacher[j - 1]]) << " " << val << " " << opt_func(values[other_teacher[j]]) << endl;
                            break;
                        }
                    }
                    if (flag == 0 && t > 1)
                    {
                        groups[other_teacher[t - 2]].push_back(values[i]);
                        stud_grp[i] = t - 1;
                        // cout << i << " " << other_teacher[t - 2] << " " << opt_func(values[other_teacher[t - 2]]) << " " << val << endl;
                    }
                }
            }
        }

        // Mapping each teacher to the group of students
        // cout << "Mapping" << endl;
        for (auto x : stud_grp)
        {
            if (x.second == 0)
            {
                teach_grp[chief_teacher].push_back(x.first);
                // cout << chief_teacher << "   " << x.first << endl;
            }
            else
            {
                teach_grp[other_teacher[x.second - 1]].push_back(x.first);
                // cout << other_teacher[x.second - 1] << "   " << x.first << endl;
            }
        }

        // Step 6+7
        cout << "Step 6+7" << endl;
        vector<vector<double>> elite;                // elite solution of each group
        vector<vector<double>> means(groups.size()); // mean of each group

        for (int i = 0; i < t; i++) // itterating over all teachers
        {
            int key;
            if (i == 0)
                key = chief_teacher;
            else
                key = other_teacher[i - 1];

            vector<double> elite_sol = groups[key][0]; // randomly selecting the first student as elite solution
            means[i] = groups[key][0];
            double gsize = groups[key].size(); // number of students in the group

            // cout << "Group size: " << gsize << endl;

            for (int j = 1; j < gsize; j++) // itterating over all students in the group
            {
                // part of step 6
                if ((opt == 1 && opt_func(groups[key][j]) > opt_func(elite_sol)) || (opt == 0 && opt_func(groups[key][j]) < opt_func(elite_sol)))
                {
                    elite_sol = groups[key][j];
                }

                // part of step 7
                for (int k = 0; k < m; k++) // itterating over all subjects
                {
                    means[i][k] += groups[key][j][k];
                    // ith groups, kth subject, jth student

                    if (j == gsize - 1) // calculating mean of the group after adding the last student
                        means[i][k] /= gsize;
                }
            }
            elite.push_back(elite_sol);
        }

        // step 8
        cout << "Step 8" << endl;
        vector<vector<double>> diff_mean;
        for (int i = 0; i < t; i++) // itterating over all teachers
        {
            vector<double> sub_diff_mean;
            vector<double> teach;

            // selecting the teacher
            if (i == 0)
                teach = values[chief_teacher]; // row of the teacher
            else
                teach = values[other_teacher[i - 1]];

            // calculating the teaching factor
            int sizee = ((i == 0) ? groups[chief_teacher].size() : groups[other_teacher[i - 1]].size());
            double TF;
            if (opt_func(teach) == 0)
                TF = 1;
            else
            {
                if (i == 0)
                    TF = abs(opt_func(groups[chief_teacher][rand() % sizee]) / opt_func(teach)); //(can be changed later)randomly selecting only one student to evaluate performace of teacher's teaching
                else
                    TF = abs(opt_func(groups[other_teacher[i - 1]][rand() % sizee]) / opt_func(teach));
            }

            for (int j = 0; j < means[i].size(); j++) // itterating over all subjects
            {
                // multiply by random value between 0 and 1 ???
                double r = double(rand() % 100) / 100;
                double diff = r * (teach[j] - TF * means[i][j]);
                sub_diff_mean.push_back(diff);
            }
            diff_mean.push_back(sub_diff_mean);
        }

        // step 9
        cout << "Step 9" << endl;
        for (int i = 0; i < values.size(); i++) // itterating over all students
        {
            vector<double> friendo = values[i];
            int key;
            if (stud_grp[i] == 0) // checking if student is in chief teacher's group
                key = chief_teacher;
            else
                key = other_teacher[stud_grp[i] - 1];

            // If there is only one student in the group, then the student will learn from himself/herself
            // Put everything in one set
            set<vector<double>> unique_solutions;
            for (auto x : groups[key])
            {
                unique_solutions.insert(x);
            }

            if (unique_solutions.size() > 1)
            {
                // find a student in the group who is not himself/herself
                while (friendo == values[i])
                    friendo = groups[key][(rand() % groups[key].size())]; // assuming each student learns only from one student in their group during tutorial
            }

            if ((opt == 1 && opt_func(friendo) > opt_func(values[i])) || (opt == 0 && opt_func(friendo) < opt_func(values[i])))
            {
                for (int j = 0; j < values[i].size(); j++) // itterating over all subjects
                {                                          // if student learning each subject from his random friends then switch the if and for statements
                    double r = double(rand() % 100) / 100;
                    values[i][j] = (values[i][j] + diff_mean[stud_grp[i]][j]) + r * (friendo[j] - values[i][j]);
                    // Wrong formula (CHnage to diff_mean[i][j] instead of means[stud_grp[i]][j])
                }
            }
            else
            {
                for (int j = 0; j < values[i].size(); j++)
                { // if student learning each subject from his random friends then switch the if and for statements
                    double r = double(rand() % 100) / 100;
                    values[i][j] = (values[i][j] + diff_mean[stud_grp[i]][j]) + r * (values[i][j] - friendo[j]);
                    // Wrong formula (CHnage to diff_mean[i][j] instead of means[stud_grp[i]][j])
                }
            }
        }

        // Step 10
        cout << "Step 10" << endl;
        for (int j = 0; j < values.size(); j++) // itterating over all students
        {
            vector<double> teach;
            vector<double> friendo = values[j];
            int key;
            if (stud_grp[j] == 0) // checking if student is in chief teacher's group
            {
                key = chief_teacher;
                teach = values[chief_teacher];
            }
            else
            {
                key = other_teacher[stud_grp[j] - 1];
                teach = values[other_teacher[stud_grp[j] - 1]];
            }

            double EF = round(1 + double(rand() % 100) / 100); // exploration factor

            // If there is only one student in the group, then the student will learn from himself/herself
            // Put everything in one set
            set<vector<double>> unique_solutions;
            for (auto x : groups[key])
            {
                unique_solutions.insert(x);
            }

            if (unique_solutions.size() > 1)
            {
                while (friendo == values[j])
                    friendo = groups[key][(rand() % groups[key].size())];
            }

            for (int k = 0; k < m; k++) // itterating over all subjects
            {
                if (opt == 1)
                {
                    double r1 = double(rand() % 100) / 100;
                    double r2 = double(rand() % 100) / 100;
                    values[j][k] = values[j][k] + r1 * (teach[k] - EF * values[j][k]) + r2 * abs(friendo[k] - values[j][k]);
                }
                else
                {
                    double r1 = double(rand() % 100) / 100;
                    double r2 = double(rand() % 100) / 100;
                    values[j][k] = values[j][k] + r1 * (EF * values[j][k] - teach[k]) + r2 * abs(friendo[k] - values[j][k]);
                    // can take abs on the abov linee
                }
            }
        }

        // Step 11
        // Replace the worst solution of each group with an elite solution.
        cout << "Step 11" << endl;
        for (int i = 0; i < t; i++) // itterating over all teachers
        {
            int key;
            if (i == 0)
                key = chief_teacher;
            else
                key = other_teacher[i - 1];

            int worst = 0;
            for (int j = 1; j < teach_grp[key].size(); j++) // itterating over all students in the group
            {
                // find the worst student in the group
                if ((opt == 1 && opt_func(values[teach_grp[key][j]]) < opt_func(values[teach_grp[key][worst]])) || (opt == 0 && opt_func(values[teach_grp[key][j]]) > opt_func(values[teach_grp[key][worst]])))
                    worst = j;
            }

            values[teach_grp[key][worst]] = elite[i]; // replace the worst student with the elite student
        }

        // step 11.5
        //  makeing sure the values are in the range
        for (int i = 0; i < values.size(); i++)
        {
            for (int j = 0; j < values[i].size(); j++)
            {
                if (values[i][j] < lb)
                    values[i][j] = lb;
                if (values[i][j] > ub)
                    values[i][j] = ub;
            }
        }

        // Step 12
        // Eliminate the duplicate solutions randomly
        cout << "Step 12" << endl;
        set<vector<double>> unique_solutions;
        for (int i = 0; i < values.size(); i++) // itterating over all students
        {
            if (unique_solutions.find(values[i]) == unique_solutions.end())
                unique_solutions.insert(values[i]);
            else
            {
                // randomly insert or not
                if (rand() % 2)
                    unique_solutions.insert(values[i]);
            }
        }

        // Step 13
        // Combine all the groups
        cout << "Step 13" << endl;
        vector<vector<double>> new_values;
        for (auto x : unique_solutions)
        {
            new_values.push_back(x);
        }

        old_values = new_values;
        values = new_values;
    }

    cout << "Final values" << endl;
    for (int i = 0; i < values.size(); i++)
    {
        for (int j = 0; j < values[i].size(); j++)
            cout << values[i][j] << " ";
        cout << endl;
    }

    return 0;
}

/*
8
2
1
0
30
1 0
2 3
-2 4
3 1
4 2
5 -1
6 -3
1 4
*/
