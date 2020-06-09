#pragma once
// A C++ program to find convex hull of a set of facilitys
// Refer https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
// for explanation of orientation()

#include <stack>

void generate_loc(facility& a, facility& b, std::vector<facility>& st);
void convexHull(std::vector<facility> &facilitys, int n);

 facility p0;
 
// A utility function to find next to top in a stack
facility nextToTop(stack<facility> &S)
{
    facility p = S.top();
    S.pop();
    facility res = S.top();
    S.push(p);
    return res;
}


 
// A utility function to swap two facilitys
void swap(facility &p1, facility &p2)
{
    facility temp = p1;
    p1 = p2;
    p2 = temp;
}

 
// A function used by library function qsort() to sort an array of
// facilitys with respect to the first facility
int compareHull(const void *vp1, const void *vp2)
{
    facility *p1 = (facility *) vp1;
    facility *p2 = (facility *) vp2;
 
    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (findDistance(p0.CoordX, p0.CoordY, (*p2).CoordX,(*p2).CoordY) >= findDistance(p0.CoordX, p0.CoordY, (*p1).CoordX, (*p1).CoordY)) ? -1 : 1;
 
    return (o == 2) ? -1 : 1;
}
 
// Prints convex hull of a set of n facilitys.
void convexHull(std::vector<facility> &facilitys, int n)
{
    std::stack<facility> S;

    if (n > 3)
    {
        // Find the bottommost facility
        int ymin = facilitys[0].CoordY, min = 0;
        for (int i = 1; i < n; i++)
        {
            int y = facilitys[i].CoordY;

            // Pick the bottom-most or chose the left most facility in case of tie
            if ((y < ymin) || (ymin == y && facilitys[i].CoordX < facilitys[min].CoordX))
                ymin = facilitys[i].CoordY, min = i;
        }

        // Place the bottom-most facility at first position
        swap(facilitys[0], facilitys[min]);
        // Sort n-1 facilitys with respect to the first facility.  A facility p1 comes
        // before p2 in sorted output if p2 has larger polar angle (in
        // counterclockwise direction) than p1
        p0 = facilitys[0];
        qsort(&facilitys[1], n - 1, sizeof(facility), compareHull);
        // Create an empty stack and push first three facilitys to it.

        S.push(facilitys[0]);
        S.push(facilitys[1]);
        S.push(facilitys[2]);
        // Process remaining n-3 facilitys
        for (int i = 3; i < n; i++)
        {
            // Keep removing top while the angle formed by facilitys next-to-top,
            // top, and facilitys[i] makes a non-left turn
            while (orientation(nextToTop(S), S.top(), facilitys[i]) == 1) //If all collinear, it pops all and return error *Orhan
            {
                if (orientation(nextToTop(S), S.top(), facilitys[i]) == 0)
                    std::cout << "Colinear" << std::endl;
                S.pop();
                //std::cout << "POPPED" << std::endl;
            }
            S.push(facilitys[i]);
            //std::cout << "PUSHED" << std::endl;
            //std::cout << i << ".Added Facilitys CoordX: " << S.top().CoordX << " CoordY: " << S.top().CoordY << std::endl;
        }
    } //endof if(n > 3)
    else
    {
        S.push(facilitys[0]);
        S.push(facilitys[1]);
        S.push(facilitys[2]);
    }
    std::vector<facility> p;
    int c = 0;
    // Now stack has the output facilitys, print contents of stack
    while (!S.empty())
    {
        p.push_back(S.top());
        //cout << "(" << p[c].CoordX << ", " << p[c].CoordY << ")" << endl;
        S.pop();
        c += 1;
    }
    //cout << "Mutation-3::Find Convex-Hull: COMPLETED" << std::endl;

    std::vector<facility> possible_loc;   //If all facilities are away greater than 2*fp it returns none of possible locations 

    int size = p.size();

        for (int i = 0; i < size; i++)
        {
            //std::cout << i << ".Convex Hull Element CoordX: " << p[i].CoordX << " Coord Y: " << p[i].CoordY << " Size: " << size << std::endl;
            if (i == size - 1)
            {
                generate_loc(p[0], p[i], possible_loc);
            }
            else
            {
                generate_loc(p[i], p[i + 1], possible_loc);
            }
        }
        //cout << "Mutation-3::Generate Possible Facility Locations: COMPLETED" << std::endl;
    int max = 0;
    int maxInd = 0;
    if (!possible_loc.empty())     //We may not generate any possible locations if each distance betweem convexhull facilities are greater than 2*fp
    {
        for (int i = 0; i < possible_loc.size(); i++)
        {
            if (!isInside(p, p.size(), possible_loc[i]))
            {
                find_fcov(possible_loc[i]);
                //std::cout << "Outside Possible Locations----- CoordX: " << possible_loc[i].CoordX << " CoordY: " << possible_loc[i].CoordY << "FacCov: " << possible_loc[i].facCov << std::endl;
                if (possible_loc[i].facCov > max) {
                    max = possible_loc[i].facCov;
                    maxInd = i;
                }

            }
        }
    facilitys.push_back(possible_loc[maxInd]); //Add max faccov facility to individual
    /*cout << "Add facility CoordX: " << possible_loc[maxInd].CoordX << " CoordY: " << possible_loc[maxInd].CoordY <<
        " FacCov: " << possible_loc[maxInd].facCov <<" New Facility Set size: "<<facilitys.size()<< endl;*/
    }
    //cout << "Mutation-3: COMPLETED" << std::endl;

}
 
/// Generation possible locations for M3

//https://math.stackexchange.com/questions/50661/analytic-geometry-point-coordinates-same-distance-from-two-points

void generate_loc(facility& a, facility& b, std::vector<facility> &st) {
    facility temp1, temp2;

    //If distance  of two facilities is greater than fp, it resuls -nan values
    if (findDistance(a.CoordX, a.CoordY, b.CoordX, b.CoordY) <= 2 * rd)
    {
        temp1.CoordX = 0.5 * (a.CoordX + b.CoordX) + sqrt(pow((float)rd, 2) / (pow((b.CoordX - a.CoordX), 2) + pow((b.CoordY - a.CoordY), 2)) - 0.25) * (b.CoordY - a.CoordY);
        temp1.CoordY = 0.5 * (a.CoordY + b.CoordY) + sqrt(pow((float)rd, 2) / (pow((b.CoordX - a.CoordX), 2) + pow((b.CoordY - a.CoordY), 2)) - 0.25) * (a.CoordX - b.CoordX);
   
        if (temp1.CoordX >= 0 && temp1.CoordY >= 0)
        {
            st.push_back(temp1);
        }

        temp2.CoordX = 0.5 * (a.CoordX + b.CoordX) - sqrt(pow((float)rd, 2) / (pow((b.CoordX - a.CoordX), 2) + pow((b.CoordY - a.CoordY), 2)) - 0.25) * (b.CoordY - a.CoordY);
        temp2.CoordY = 0.5 * (a.CoordY + b.CoordY) - sqrt(pow((float)rd, 2) / (pow((b.CoordX - a.CoordX), 2) + pow((b.CoordY - a.CoordY), 2)) - 0.25) * (a.CoordX - b.CoordX);

        if (temp2.CoordX >= 0 && temp2.CoordY >= 0)
        {
            st.push_back(temp2);
        }
        /*std::cout << "Generated Locations 1.CoordX: " << temp1.CoordX << " CoordY: " << temp1.CoordY <<
            " 2.CoordX: " << temp2.CoordX << " CoordY: " << temp2.CoordY << std::endl;*/
    }


}