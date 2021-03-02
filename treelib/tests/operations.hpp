namespace tree::testing
{
    enum class eOperation : char {insert, erase, find};

    eOperation get_operation(const int random_value)
    {
        if (random_value >= 0 && random_value < 50)
        {
            return eOperation::insert;
        }
        else if (random_value >= 50 && random_value < 75)
        {
            return eOperation::erase;
        }
        else if (random_value >= 75 && random_value < 100)
        {
            return eOperation::find;
        }
        else
        {
            throw std::runtime_error("wrong random value for eOperation");
        }
    }

}