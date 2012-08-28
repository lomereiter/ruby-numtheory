require 'numtheory'

describe "bit functions" do
  describe "popcount" do
    it "should return the number of bits set in a non-negative number" do
      12351231231235091328209183029825132908123295012351.popcount.should == 84
      82362346211236.popcount.should == 23
    end

    it "should raise ArgumentError for negative numbers" do
      expect { (-5).popcount }.should raise_error
      expect { (-112351235123512351235).popcount }.should raise_error
    end
  end

  describe "bitlength" do
    it "should return the length of a positive number in bits" do
      123512351235125312.bitlength.should == 57
      12355312.bitlength.should == 24
      9123616312361235523462346311236136123123136212361.bitlength.should == 163
    end

    it "should return 1 when argument is 0" do
      0.bitlength.should == 1
    end

    it "should raise ArgumentError for negative numbers" do
      expect { (-12351).bitlength }.should raise_error
      expect { (-2039462306492384059132).bitlength }.should raise_error
    end
  end
end
