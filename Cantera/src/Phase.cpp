/**
 *  @file Phase.cpp
 */

// Copyright 2001  California Institute of Technology

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "ct_defs.h"
#include "Phase.h"
#include "vec_functions.h"
#include "ctexceptions.h"

namespace Cantera {    

        void Phase::saveState(vector_fp& state) const {
            state.resize(nSpecies() + 2);
            saveState(state.size(),state.begin());
        }
        void Phase::saveState(int lenstate, doublereal* state) const {
            state[0] = temperature();
            state[1] = density();
            getMassFractions(state + 2);
        }

        void Phase::restoreState(vector_fp& state) {
            restoreState(state.size(),state.begin());
        }

        void Phase::restoreState(int lenstate, doublereal* state) {
            if (int(lenstate) >= nSpecies() + 2) {
                setMassFractions_NoNorm(state + 2);
                setTemperature(state[0]);
                setDensity(state[1]);
            }
            else {
                throw ArraySizeError("Phase::restoreState",
                    lenstate,nSpecies()+2);
            }
        }

        void Phase::setMoleFractionsByName(compositionMap& xMap) {
            int kk = nSpecies();
            doublereal x;
            vector_fp mf(kk, 0.0);
            for (int k = 0; k < kk; k++) {
                x = xMap[speciesName(k)];
                if (x > 0.0) mf[k] = x;
            }
            setMoleFractions(mf.begin());
        }

        void Phase::setMoleFractionsByName(const string& x) {
            compositionMap xx;
            int kk = nSpecies();
            for (int k = 0; k < kk; k++) { 
                xx[speciesName(k)] = -1.0;
            }
            parseCompString(x, xx);
            setMoleFractionsByName(xx);
            //int kk = nSpecies();
            //vector_fp mf(kk);
            //for (int k = 0; k < kk; k++) { 
            //    mf[k] = xx[speciesName(k)];
            //}
            //setMoleFractions(mf.begin());
        }

        void Phase::setMassFractionsByName(compositionMap& yMap) {
            int kk = nSpecies();
            doublereal y;
            vector_fp mf(kk, 0.0);
            for (int k = 0; k < kk; k++) { 
                y = yMap[speciesName(k)];
                if (y > 0.0) mf[k] = y;
            }
            setMassFractions(mf.begin());
        }

        void Phase::setMassFractionsByName(const string& y) {
            compositionMap yy;
            int kk = nSpecies();
            for (int k = 0; k < kk; k++) { 
                yy[speciesName(k)] = -1.0;
            }
            parseCompString(y, yy);
            setMassFractionsByName(yy);
        }

        /** Set the temperature (K), density (kg/m^3), and mole fractions. */
        void Phase::setState_TRX(doublereal t, doublereal dens, 
            const doublereal* x) {
            setMoleFractions(x); setTemperature(t); setDensity(dens);
        }

        /** Set the temperature (K), density (kg/m^3), and mole fractions. */
        void Phase::setState_TRX(doublereal t, doublereal dens, 
            compositionMap& x) {
            setMoleFractionsByName(x); setTemperature(t); setDensity(dens);
        }

        /** Set the temperature (K), density (kg/m^3), and mass fractions. */
        void Phase::setState_TRY(doublereal t, doublereal dens, 
            const doublereal* y) {
            setMassFractions(y); setTemperature(t); setDensity(dens);
        }        

        /** Set the temperature (K), density (kg/m^3), and mass fractions. */
        void Phase::setState_TRY(doublereal t, doublereal dens, 
            compositionMap& y) {
            setMassFractionsByName(y); setTemperature(t); setDensity(dens);
        }
    
        /** Set the temperature (K) and density (kg/m^3) */
        void Phase::setState_TR(doublereal t, doublereal rho) {
            setTemperature(t); setDensity(rho);
        }
    
        /** Set the temperature (K) and mole fractions.  */
        void Phase::setState_TX(doublereal t, doublereal* x) {
            setTemperature(t); setMoleFractions(x);
        }

        /** Set the temperature (K) and mass fractions.  */
        void Phase::setState_TY(doublereal t, doublereal* y) {
            setTemperature(t); setMassFractions(y);
        }

        /** Set the density (kg/m^3) and mole fractions.  */
        void Phase::setState_RX(doublereal rho, doublereal* x) {
            setMoleFractions(x); setDensity(rho);
        }

        /** Set the density (kg/m^3) and mass fractions.  */
        void Phase::setState_RY(doublereal rho, doublereal* y) {
            setMassFractions(y); setDensity(rho);
        }

        /**
         * Copy the vector of molecular weights into vector weights.
         */
        void Phase::getMolecularWeights(vector_fp& weights) {
            const array_fp& mw = Constituents::molecularWeights();
            if (weights.size() < mw.size()) weights.resize(mw.size());
            copy(mw.begin(), mw.end(), weights.begin());
        }

        /**
         * Copy the vector of molecular weights into array weights.
	 * @deprecated
         */
        void Phase::getMolecularWeights(int iwt, doublereal* weights) {
            const array_fp& mw = Constituents::molecularWeights();
            copy(mw.begin(), mw.end(), weights);
	}

       /**
         * Copy the vector of molecular weights into array weights.
         */
        void Phase::getMolecularWeights(doublereal* weights) {
            const array_fp& mw = Constituents::molecularWeights();
            copy(mw.begin(), mw.end(), weights);
        }

        /**
         * Return a const reference to the internal vector of
         * molecular weights.
         */
        const array_fp& Phase::molecularWeights() {
            return Constituents::molecularWeights(); 
        }


        /**
         * Get the mole fractions by name. 
         */
        void Phase::getMoleFractionsByName(compositionMap& x) {
            x.clear();
            int kk = nSpecies();
            for (int k = 0; k < kk; k++) {
                x[speciesName(k)] = State::moleFraction(k);
            }
        }

        doublereal Phase::moleFraction(int k) const {
            return State::moleFraction(k);
        }

        doublereal Phase::moleFraction(string name) const {
            int iloc = speciesIndex(name);
            if (iloc >= 0) return State::moleFraction(iloc);
            else return 0.0;
        }

        doublereal Phase::massFraction(int k) const {
            return State::massFraction(k);
        }

        doublereal Phase::massFraction(string name) const {
            int iloc = speciesIndex(name);
            if (iloc >= 0) return massFractions()[iloc];
            else return 0.0;
        }

    doublereal Phase::chargeDensity() const {
        int k;
        int nsp = nSpecies();
        doublereal cdens = 0.0;
        for (k = 0; k < nsp; k++) 
            cdens += charge(k)*State::moleFraction(k);
        cdens *= Faraday;
        return cdens;
    }


//         void Phase::update_T(int n) const {
//             m_T_updater.update(n);
//         }

//         void Phase::update_C(int n) const {
//             m_C_updater.update(n);
//         }

        /** 
         *  Finished adding species, prepare to use them for calculation
         *  of mixture properties.
         */
        void Phase::freezeSpecies() {
            Constituents::freezeSpecies();
            init(Constituents::molecularWeights());
            int kk = nSpecies();
            int nv = kk + 2;
            m_data.resize(nv,0.0);
            m_data[0] = 300.0;
            m_data[1] = 0.001;
            m_data[2] = 1.0;

            setState_TRY(300.0, density(), m_data.begin() + 2);

            m_kk = nSpecies();
	} 

        bool Phase::ready() const {
            return (m_kk > 0 && Constituents::ready() && State::ready());
        }

//         int Phase::installUpdater_T(Updater* u) {
//             return m_T_updater.install(u);
//         }

//         int Phase::installUpdater_C(Updater* u) {
//             return m_C_updater.install(u);
//         }
}
