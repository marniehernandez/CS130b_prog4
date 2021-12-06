#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <random>         // for our random algorithm
//#include <time.h>       // to get the runtime for inputs

using namespace std;

#include <chrono>       // to get the runtime for inputs. Taken from https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
using namespace std::chrono;

class Coordinate{

    public:

    int identifier;
    double x;
    double y;

    Coordinate()
    {
        identifier = -1;
        x = 0;
        y = 0;
    }

    Coordinate(int i, double x_pos, double y_pos)
    {
        identifier = i;
        x = x_pos;
        y = y_pos;
    }

    Coordinate( const Coordinate &c )
    {
        identifier = c.identifier;
        x = c.x;
        y = c.y;
    }

    void printWithId()
    {
        cout << identifier << ", " << x << ", " << y << "\n";
    }
};



class Line{

    public:

    double slope;
    double y_intercept;
    Coordinate a;
    Coordinate b;

    Line(Coordinate c1, Coordinate c2)
    {
        slope = (c2.y - c1.y)/(c2.x - c1.x);
            //  y-y1 = m*(x-x1)
            //  y = m*(x-x1) + y1
        y_intercept = c1.y - (slope*c1.x);
        a = c1;
        b = c2;
    }
    
    Line(double m, double intercept, Coordinate c)
    {
        slope = m;
        y_intercept = intercept;
        a = c;
        b = c;
    }
    
    Line( const Line &l )
    {
        slope = l.slope;
        y_intercept = l.y_intercept;
        a = l.a;
        b = l.b;
    }
    
    void operator = ( const Line &l )
    { 
        slope = l.slope;
        y_intercept = l.y_intercept;
        a = l.a;
        b = l.b;
    }

    void printEquation()
    {
        cout << "y = " << slope << "x + " << y_intercept << "\n";
    }

    void printEndpoints()
    {
        cout << "Endpoints of ";
        printEquation();
        a.printWithId();
        b.printWithId();
    }
    
    double getDistanceFrom(Coordinate c)
    {
        //taken from https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
        //  ax + by + c = 0
        //  by = -ax - c
        //  a = -slope
        //  b = 1
        //  c = -y_intercept
        //positve distance is above the line, negative is below
        /*
             | k + m*x_0 + y_0 |
            ---------------------
                 sqrt(1+m^2)

            m = -a/b = -slope
            k = -c/b = -y_intercept
            x_0 = c.x; y_0 = c.y

               | -y_intercept -slope*c.x + c.y |
            = --------------------------------
                   sqrt(1+m^2)
        */
        double numerator = -(slope * c.x) + c.y - y_intercept;
        double denominator = pow(slope,2) + 1;

        return numerator/sqrt(denominator);
    }
};





int calculate_middle(int num_points)
{
    if( num_points % 2 == 1 )
        return (num_points-1)/2;
    return num_points/2;
}

vector< std::tuple<double, int> > distancesFromLine(Line l, vector< Coordinate > coordinates, double endpoint_1, double endpoint_2)
{
    vector< std::tuple<double, int> > list_of_distances;
    for(int i = 0; i<coordinates.size(); i++)
    {
        std::tuple<double, int> dist;

        if( ( i != endpoint_1 ) && ( i != endpoint_2 ) )
        {
            std::get<0>(dist) = abs(l.getDistanceFrom(coordinates[i]));
            std::get<1>(dist) = coordinates[i].identifier;
            list_of_distances.push_back(dist);
        }
        else
        {
            std::get<0>(dist) = 0;
            std::get<1>(dist) = coordinates[i].identifier;
            list_of_distances.push_back(dist);
        }
    }
    return list_of_distances;
}

std::tuple<double, double> mean( vector< Coordinate > coordinates )
{
    double x_sum = 0.0;
    double y_sum = 0.0;
    int x_total = 0;
    int y_total = 0;
    std::tuple<double, double> means;
    for (vector< Coordinate >::iterator it = coordinates.begin(); it != coordinates.end(); it++)
    {
        x_sum += it->x;
        y_sum += it->y;
        x_total++;
        y_total++;
    }
    std::get<0>(means) = x_sum/x_total;
    std::get<1>(means) = y_sum/y_total;
    
    return means;
}

Line best_fit( vector< Coordinate > coordinates )
{
    bool empty = true;
    double numerator = 0;
    double denominator = 0;
    
    //std::get<0>(means) is x mean
    //std::get<1>(means) is y mean
    std::tuple<double, double> means = mean( coordinates );
    
    for (vector< Coordinate >::iterator it = coordinates.begin(); it != coordinates.end(); it++)
    {
        numerator += ( it->x - std::get<0>(means)) * ( it->y - std::get<1>(means));
        denominator += ( it->x - std::get<0>(means)) * ( it->x - std::get<0>(means));
        empty = false;
    }
    double m = numerator/denominator;
    double intercept = std::get<0>(means) - ( m * (std::get<1>(means)) );
    Line l(m, intercept, coordinates[0]);
    return l;
}





int main(int argc, char *argv[])
{    

    if (argc > 1)
    {
        std::cerr << "Usage: " << argv[0] << " < input" << std::endl;
        return 1;
    }
    //obtain the coordinates of all points as a pair
    auto start = high_resolution_clock::now();
    
    vector< Coordinate > coordinates;

    std::string input = "a";
    int num_points = 0;
    double x_input;
    double y_input;
    int random_1 = 0;
    int random_2 = 0;
    int middle = 0;
    
    while( input != "" )
    {
        getline(cin, input);
        if(input == "")
        {
            continue;
        }
        x_input = stod( input.substr(0, input.find(" ")) );
        y_input = stod( input.substr(input.find(" ")+1) );
        
        Coordinate coord_input(num_points, x_input, y_input);
        coordinates.push_back( coord_input );
        num_points++;
    }
    //middle of our array
    middle = calculate_middle(num_points);
    
    //from https://www.cplusplus.com/reference/random/linear_congruential_engine/operator()/
    //creates a seed at the current time on the system clock, and then a number generator
    //also generate two numbers no greater than the num_points
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);
    
    //line with the minimum error
    Line best_line(coordinates[0], coordinates[1]);
    //the list of errors given our line of best fit
    vector< std::tuple<double, int> > best_line_errors;
    //the lowest median error of our lists
    double lowest_median_error = DBL_MAX;
    int lowest_median_error_index = -1;
    
    //Generate 20 samples, and only pick the line with lowest median error
    //Probability of including one outlier in the sample, assuming worst-case (30% points
    //   are outliers; epsilon = 0.3) is (2*0.3*0.7) = 0.42
    //Probability of including two outliers is (0.3*0.3) = 0.09
    //Probability of including any # of outliers is 0.42+0.09 = 0.51
    //Probability of including no outliers is only 0.49, less than half probability
    //Probability of not including a sample with no outliers, out of 16 samples,
    //   is (0.51)^20 = 0.00000141710986708
    //Also need to account samples that are not outliers but are at a steep angle compared to the lbf
    int i_max = (int)(32 * log(num_points));
    for(int i = 0; i < i_max; i++)
    {
        random_1 = generator() % num_points;
        random_2 = generator() % (num_points-1);
        if(random_2 >= random_1)
        {
            random_2++;
        }
        
        //assign our random values to coordinates
        Coordinate c1 = coordinates[random_1];
        Coordinate c2 = coordinates[random_2];
        
        //new line of best fit to compare with our old data
        Line new_line(c1, c2);

        vector< std::tuple<double, int> > d = distancesFromLine(new_line, coordinates, random_1, random_2);
        //sort this list based on the distances from the line while preserving their index
        std::sort(d.begin(), d.end());
        //get median
        int new_index = std::get<1>( d[middle] );
        double new_median_error = std::get<0>( d[middle] );
        
        if(new_median_error < lowest_median_error)
        {
            lowest_median_error = new_median_error;
            lowest_median_error_index = new_index;
            best_line = new_line;
            best_line_errors = d;
        }
    }
    
    //create a new list of coordinates only containing the data less than the median error
    vector< Coordinate > best_coordinates;
    //randomly get a line from this set again, and choose the smallest error
    
    for(int i = 0; i < middle; i++)
    {
        best_coordinates.push_back(  coordinates[ std::get<1>(best_line_errors[i]) ] );
    }
    //for (vector< Coordinate >::iterator it = best_coordinates.begin(); it != best_coordinates.end(); it++)
    //{
    //    it->printWithId();
    //}
    //best_line.printEquation();
    
    //perform line of best fit
    Line lbf = best_fit( best_coordinates );
    
    float m = (float)lbf.slope;
    float y_int = (float)lbf.y_intercept;

    cout << m << " " << y_int;
    
    auto end = high_resolution_clock::now();
    auto total_runtime = duration_cast<microseconds>(end - start);
    double runtime_in_seconds = (double)total_runtime.count() / 1000000.0;
    //cout << "Runtime: " << runtime_in_seconds << " s\n";
    
    return 0;
}
