require 'numtheory'

describe "arithmetic function" do
  include NumTheory

  describe "primepi" do
    it "should raise ArgumentError if there're not enough precomputed primes" do
      expect { primepi(100_000_000_000) }.to raise_error(ArgumentError)
    end

    it "should return the number of primes less or equal than a given number" do
      expect(primepi(1)).to eq(0)
      expect(primepi(2)).to eq(1)
      expect(primepi(123512)).to eq(11607)
      expect(primepi(498765)).to eq(41438)
      expect(primepi(312361)).to eq(26983)
      expect(primepi(-123513)).to eq(0)
    end
  end

  describe "eulerphi" do
    it "should raise ArgumentError if the number is zero" do
      expect { eulerphi(0) }.to raise_error(ArgumentError)
    end

    it "should return the number of positive integers less than 'n' and coprime with 'n'" do
      expect(eulerphi(12312361361)).to eq(11664342324)
      expect(eulerphi(12361212371236123)).to eq(11713214239144000)
    end
  end
end
