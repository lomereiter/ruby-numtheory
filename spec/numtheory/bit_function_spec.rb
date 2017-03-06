require 'numtheory'

describe "bit functions" do
  describe "popcount" do
    it "should return the number of bits set in a non-negative number" do
      expect(12351231231235091328209183029825132908123295012351.popcount).to eq (84)
      expect(82362346211236.popcount).to eq(23)
    end

    it "should raise ArgumentError for negative numbers" do
      expect { (-5).popcount }.to raise_error(ArgumentError)
      expect { (-112351235123512351235).popcount }.to raise_error(ArgumentError)
    end
  end

  describe "bitlength" do
    it "should return the length of a positive number in bits" do
      expect(123512351235125312.bitlength).to eq(57)
      expect(12355312.bitlength).to eq(24)
      expect(9123616312361235523462346311236136123123136212361.bitlength).to eq(163)
    end

    it "should return 1 when argument is 0" do
      expect(0.bitlength).to eq(1)
    end

    it "should raise ArgumentError for negative numbers" do
      expect { (-12351).bitlength }.to raise_error(ArgumentError)
      expect { (-2039462306492384059132).bitlength }.to raise_error(ArgumentError)
    end
  end
end
