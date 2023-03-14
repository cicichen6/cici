#include "Playlist.hpp"
#include "../Utils.hpp"
#include <cmath>

Playlist Playlist::parse(oatpp::parser::Caret& caret) {
	std::list<RecoredMarker> result;

	while (caret.canContinue()) {
	  caret.findChar('#');
	  if(caret.isAtText("#EXTINF:", true)) {
	  	v_float64 secs = caret.parseFloat64();
		caret.findROrN();
		caret.skipAllRsAndNs();
		auto uriLable = caret.putLable();
		caret.findChar('\n');
		result.push_back({secs, oatpp::String(uriLable.toString())});
	  }
	  caret.findROrN();
	  caret.skipAllRsAndNs();
	}
	return Playlist(result);
}

Playlist Playlist::parseFromFile(const char* filename){
	auto text = oatpp::String::loadFromFile(filename);
	if(!text){
	    throw std::runtime
	}
}

