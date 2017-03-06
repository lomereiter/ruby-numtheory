require 'numtheory'

describe "primality tests" do
  
  describe "prime? method" do
    it "should return exact result for precomputed values" do
      expect(0).not_to be_prime
      expect(1).not_to be_prime
      expect(2).to be_prime
      expect(3).to be_prime
      expect(9).not_to be_prime
      expect(-1).not_to be_prime
      expect(-2).to be_prime
      expect(-13).to be_prime
      expect(-16).not_to be_prime
    end

    it "should apply BPSW test for larger values" do
      expect(12351236123627).not_to be_prime
      expect(12351236123659).to be_prime
      expect(12351236123627123612361235012532906135124234123512356123512342219).not_to be_prime
      expect(12351236123627123612361235012532906135124234123512356123512342221).to be_prime
    end

    it "should return true for BPSW pseudoprimes" do
      # no one is found yet :-)
    end
  end

  describe "miller-rabin test" do
    it "should perform NumTheory::PrimalityTests rounds of Miller-Rabin test" do
      NumTheory.module_eval { remove_const :PrimalityTests }
      NumTheory::PrimalityTests = 1
      srand(100) # MR test uses random base to start
      expect(671).to be_MR_pseudoprime
      expect(671).not_to be_BPSW_pseudoprime

      NumTheory.module_eval { remove_const :PrimalityTests }
      NumTheory::PrimalityTests = 10
      srand(42)
      expect(671).not_to be_MR_pseudoprime
    end
  end
end
