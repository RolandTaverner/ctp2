









#pragma once
#ifndef __CIVILISATION_DATABASE_H__
#define __CIVILISATION_DATABASE_H__

#include "DB.h"
#include "CivilisationRec.h"

class CivArchive ;
class Token ;

enum CIV_INDEX ;
enum C3DIR;

class CivilisationDatabase : public Database<CivilisationRecord>
	{
	private:

	public:
		CivilisationDatabase () ;
		CivilisationDatabase (CivArchive &archive) ;
		~CivilisationDatabase () ;
		void Serialize(CivArchive &archive) ;

		

		BOOL Initialise(char *filename, C3DIR dir) ;
	    BOOL ParseCivilisationDatabase(char *filename, C3DIR dir);
		BOOL ParseACivilisation(Token *token, sint32 count) ;

		sint32 GetCivilisations(void) const { return (m_nRec) ; }

		StringId GetLeaderName(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetLeaderName()) ;
			}
		
		StringId GetLeaderNameFemale(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetLeaderNameFemale()) ;
			}

        StringId GetPersonalityDescription(const CIV_INDEX civ) const
        { 
            Assert(civ>=0); 
            Assert(civ<=(CIV_INDEX)(m_nRec));
            return (m_rec[civ].GetPersonalityDescription()); 
        } 


		StringId GetPluralCivName(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetPluralCivName()) ;
			}
		
		StringId GetCountryName(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetCountryName()) ;
			}

		StringId GetSingularCivName(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetSingularCivName()) ;
			}

		StringId GetEmissaryPhoto( const CIV_INDEX civ ) const
		{
			Assert( civ >= 0 );
			Assert( civ <= (CIV_INDEX)(m_nRec));
			return ( m_rec[civ].GetEmissaryPhoto() );
		}

		sint32 GetNumCities(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetNumCities()) ;
			}

		StringId GetCityName(const CIV_INDEX civ, const sint32 city)
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetCityName(city)) ;
			}

		StringId GetCapital(const CIV_INDEX civ) const
			{
			Assert(civ>=0) ; 
			Assert(civ<=(CIV_INDEX)(m_nRec)) ;
			return (m_rec[civ].GetCapital()) ;
			}

		sint32 GetCityStyle( const CIV_INDEX civ ) const {
			Assert( civ >= 0 );
			Assert( civ <= (CIV_INDEX)(m_nRec) );
			return ( m_rec[civ].GetCityStyle() );
		}

	} ;

extern CivilisationDatabase *g_theCivilisationDB ;

#endif
