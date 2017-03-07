require 'numtheory'

describe "Z/pZ" do

  describe "modular inverse" do
    it "should raise ArgumentError if the inverse doesn't exist" do
      expect { 3.inverse(1236123) }.to raise_error(ArgumentError)
      expect { 636356661312236113261221231.inverse(71324141236123712312361347131) }.to raise_error(ArgumentError)
    end

    it "should return the inverse modulo 'p' if it exists" do
      expect(170149610396103139612301.inverse(91023960139513512035130519351035)).to eq(47319168690720853552108144675271)
      expect(10239613261.inverse(1713461351236)).to eq(1314959799745)
      expect(61235.inverse(13271)).to eq(10552)
    end
  end

  describe "jacobi symbol (a|n)" do
    it "should raise ArgumentError for negative 'n'" do
      expect { 23.jacobi(-15) }.to raise_error(ArgumentError)
    end

    it "should raise ArgumentError for even 'n'" do
      expect { 17.jacobi(24) }.to raise_error(ArgumentError)
    end

    it "should return 0 when 'a' and 'n' have common divisors" do
      expect(25.jacobi(35)).to be_zero
      expect(-24.jacobi(9)).to be_zero
      expect(33.jacobi(3)).to be_zero
      expect(128367169183989283691832619381.jacobi(1829631238193612396183612312361231123)).to be_zero
    end

    it "should return 1 when 'a' is a square residue modulo 'n'" do
      expect(26.jacobi(25)).to eq(1)
      expect(261235125312351.jacobi(251235123123512351)).to eq(1)
      expect(123412354123123512351235123512455123.jacobi(12351231236123112351235123523513411)).to eq(1)
    end

    it "should return -1 in all the other cases" do
      expect(123412354123123512351235123512455123.jacobi(1235123123612311235123512352351341)).to eq(-1)
      expect(1386136123613661.jacobi(19381923138198391)).to eq(-1)
      expect(2.jacobi(5)).to eq(-1)
      expect(17.jacobi(23)).to eq(-1)
    end
  end

  describe "square root" do

    it "should raise ArgumentError if 'p' is not a prime number" do
      expect { 7.sqrt_mod(9)}.to raise_error(ArgumentError)
    end

    it "should return nil if there is no square root" do
      expect(33.sqrt_mod(3)).to be_nil
      expect(1231356123521351251.sqrt_mod(123061236013903516123121612316123612379)).to be_nil
    end

    it "should otherwise return the square root from range [0 .. p/2]" do
      expect(1235112351311.sqrt_mod(123061936032513553)).to eq(20377270042474840)
      expect(910836013912301293612035135.sqrt_mod(45734753461235122313131523613612412341612361691)).to eq(17194653522779996910096883279884862754880186510)
    end
  end
end
