require 'numtheory'

describe "arithmetic function" do
  include NumTheory

  describe "primepi" do
    it "should raise ArgumentError if there're not enough precomputed primes" do
      expect { primepi(100_000_000_000) }.should raise_error
    end

    it "should return the number of primes less or equal than a given number" do
      primepi(1).should == 0
      primepi(2).should == 1
      primepi(123512).should == 11607
      primepi(498765).should == 41438
      primepi(312361).should == 26983
      primepi(-123513).should == 0
    end
  end

  describe "eulerphi" do
    it "should raise ArgumentError if the number is zero" do
      expect { eulerphi(0) }.should raise_error
    end

    it "should return the number of positive integers less than 'n' and coprime with 'n'" do
      eulerphi(12312361361).should == 11664342324
      eulerphi(12361212371236123).should == 11713214239144000
    end
  end
end
