#pragma once

namespace Comparison
{
    long max(long lhs, long rhs);
    long min(long lhs, long rhs);
    bool fits(
        long brickWidth, 
        long brickHeight, 
        long brickLength, 
        long holeWidth, 
        long holeHeight
    );
}