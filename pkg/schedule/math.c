float ceiling (float x)
{
    float rem = x - (int)x;
    if (rem)
    {
        return 1 + (int)x;
    }
    return x;
}

float round_5 (float x)
{
    float rem = x - (int)x;
    if (rem > 0.5)
    {
        return 1 + (int)x;
    }
    return x;
}
