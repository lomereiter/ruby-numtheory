require 'numtheory'

RSpec.configure do |config|
  config.expect_with :rspec do |c|
    c.syntax = [:should]
  end
end

describe "primality tests" do
  
  describe "prime? method" do
    it "should return exact result for precomputed values" do
      0.should_not be_prime
      1.should_not be_prime
      2.should be_prime
      3.should be_prime
      9.should_not be_prime
      -1.should_not be_prime
      -2.should be_prime
      -13.should be_prime
      -16.should_not be_prime
    end

    it "should apply BPSW test for larger values" do
      12351236123627.should_not be_prime
      12351236123659.should be_prime
      12351236123627123612361235012532906135124234123512356123512342219.should_not be_prime
      12351236123627123612361235012532906135124234123512356123512342221.should be_prime
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
      671.should be_MR_pseudoprime
      671.should_not be_BPSW_pseudoprime

      NumTheory.module_eval { remove_const :PrimalityTests }
      NumTheory::PrimalityTests = 10
      srand(42)
      671.should_not be_MR_pseudoprime
    end
  end
end
