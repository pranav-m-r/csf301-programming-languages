class Class1 {
  private:
    unsigned char *p;
  public:
    Class1(int inputSize);
    int getInt(int index);
    int getDouble(int index);
    int getFloat(int index);
    void setInt(int i, int index);
    void setDouble(double d, int index);
    void setFloat(float f, int index);
};
