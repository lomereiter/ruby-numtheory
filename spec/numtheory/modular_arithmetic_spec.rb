require 'numtheory'

describe "Z/pZ" do

  describe "modular inverse" do
    it "should raise ArgumentError if the inverse doesn't exist" do
      expect { 3.inverse(1236123) }.should raise_error
      expect { 636356661312236113261221231.inverse(71324141236123712312361347131) }.should raise_error
    end

    it "should return the inverse modulo 'p' if it exists" do
      170149610396103139612301.inverse(91023960139513512035130519351035).should == 47319168690720853552108144675271
      10239613261.inverse(1713461351236).should == 1314959799745
      61235.inverse(13271).should == 10552
    end
  end

  describe "jacobi symbol (a|n)" do
    it "should raise ArgumentError for negative 'n'" do
      expect { 23.jacobi(-15) }.should raise_error
    end

    it "should raise ArgumentError for even 'n'" do
      expect { 17.jacobi(24) }.should raise_error
    end

    it "should return 0 when 'a' and 'n' have common divisors" do
      25.jacobi(35).should be_zero
      -24.jacobi(9).should be_zero
      33.jacobi(3).should be_zero
      128367169183989283691832619381.jacobi(1829631238193612396183612312361231123).should be_zero
    end

    it "should return 1 when 'a' is a square residue modulo 'n'" do
      26.jacobi(25).should == 1
      261235125312351.jacobi(251235123123512351).should == 1
      123412354123123512351235123512455123.jacobi(12351231236123112351235123523513411).should == 1
    end

    it "should return -1 in all the other cases" do
      123412354123123512351235123512455123.jacobi(1235123123612311235123512352351341).should == -1
      1386136123613661.jacobi(19381923138198391).should == -1
      2.jacobi(5).should == -1
      17.jacobi(23).should == -1
    end
  end

  describe "square root" do

    it "should raise ArgumentError if 'p' is not a prime number" do
      expect { 7.sqrt_mod(9)}.should raise_error
    end

    it "should return nil if there is no square root" do
      33.sqrt_mod(3).should be_nil
      1231356123521351251.sqrt_mod(123061236013903516123121612316123612379).should be_nil
    end

    it "should otherwise return the square root from range [0 .. p/2]" do
      1235112351311.sqrt_mod(123061936032513553).should == 20377270042474840
      910836013912301293612035135.sqrt_mod(45734753461235122313131523613612412341612361691).should == 17194653522779996910096883279884862754880186510
    end
  end
end
