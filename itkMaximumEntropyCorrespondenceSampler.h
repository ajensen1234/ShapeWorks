/*=========================================================================
  Program:   ShapeWorks: Particle-based Shape Correspondence & Visualization
  Module:    $RCSfile: itkMaximumEntropyCorrespondenceSampler.h,v $
  Date:      $Date: 2011/03/24 01:17:33 $
  Version:   $Revision: 1.3 $
  Author:    $Author: wmartin $

  Copyright (c) 2009 Scientific Computing and Imaging Institute.
  See ShapeWorksLicense.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __itkMaximumEntropyCorrespondenceSampler_h
#define __itkMaximumEntropyCorrespondenceSampler_h

#include "itkMaximumEntropySurfaceSampler.h"
#include "itkParticleDualVectorFunction.h"
#include "itkParticleEnsembleNormalPenaltyFunction.h"
#include "itkParticleEnsembleMeanFunction.h"
#include "itkParticleEnsembleEntropyFunction.h"
#include "itkParticleGeneralEntropyGradientFunction.h"

#include "itkParticleShapeLinearRegressionMatrixAttribute.h"
#include "itkParticleShapeMixedEffectsMatrixAttribute.h"

#include "itkParticleMeshBasedGeneralEntropyGradientFunction.h"
#include "itkParticleMeshBasedGeneralMeanGradientFunction.h"

namespace itk
{
  
/** \class MaximumEntropyCorrespondenceSampler
 *
 * 
 *
 */
template <class TImage>
class ITK_EXPORT MaximumEntropyCorrespondenceSampler
  : public MaximumEntropySurfaceSampler<TImage> 
{
public:
  /** Standard class typedefs. */
  typedef MaximumEntropyCorrespondenceSampler  Self;
  typedef MaximumEntropySurfaceSampler<TImage>  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MaximumEntropyCorrespondenceSampler, MaximumEntropySurfaceSampler);

  /**Expose the image dimension. */
  itkStaticConstMacro(Dimension, unsigned int, TImage::ImageDimension);

  /** Type of the input/output image. */
  typedef typename Superclass::ImageType ImageType;

  /** Expose the point type */
  typedef typename ImageType::PointType PointType;


  void SetCorrespondenceOn()
  {
    m_LinkingFunction->SetBOn();
    this->Modified();
  }
  void SetCorrespondenceOff()
  {
    m_LinkingFunction->SetBOff();
    this->Modified();
  }

  void SetNormalEnergyOn()
  {
    m_LinkingFunction->SetCOn();
    this->Modified();
  }

  void SetNormalEnergyOff()
  {
    m_LinkingFunction->SetCOff();
    this->Modified();
  }

  void SetSamplingOn()
  {
    m_LinkingFunction->SetAOn();
    this->Modified();
  }
  void SetSamplingOff()
  {
    m_LinkingFunction->SetAOff();
    this->Modified();
  }

  bool GetCorrespondenceOn() const
  { return m_LinkingFunction->GetBOn(); }
  bool GetSamplingOn() const
  { return m_LinkingFunction->GetAOn(); }

  /** This method sets the optimization function for the sampling.
      mode 0 = isotropic adaptivity
      mode 1 = no adaptivity
  */
  virtual void SetAdaptivityMode(int mode)
  {
    if (mode == 0)
      {
        // PRATEEP
        if(this->m_pairwise_potential_type == 0)
            m_LinkingFunction->SetFunctionA(this->GetCurvatureGradientFunction());
        else if(this->m_pairwise_potential_type == 1)
            m_LinkingFunction->SetFunctionA( this->GetModifiedCotangentGradientFunction());
      }
    else if (mode == 1)
      {
      m_LinkingFunction->SetFunctionA(this->GetGradientFunction());
      }
    else if (mode == 2)
      {
      m_LinkingFunction->SetFunctionA(this->GetQualifierGradientFunction());
      }
    else if (mode == 3)
      {
        if(this->m_pairwise_potential_type == 0)
            m_LinkingFunction->SetFunctionA(this->GetOmegaGradientFunction());
        else if(this->m_pairwise_potential_type == 1)
            m_LinkingFunction->SetFunctionA(this->GetConstrainedModifiedCotangentGradientFunction());
      }

    Superclass::m_AdaptivityMode = mode;
    this->Modified();
  }

  /** This method sets the optimization function for correspondences between
      surfaces (domains).
      mode 0 = mean force
      mode 1 = minimum entropy
  */
  virtual void SetCorrespondenceMode(int mode)
  {
    if (mode == 1)
    {
      m_LinkingFunction->SetFunctionB(m_EnsembleEntropyFunction);
    }
    else if (mode == 2)
    {
      m_LinkingFunction->SetFunctionB(m_GeneralEntropyGradientFunction);
    }
    else if (mode == 3)
    {
      m_LinkingFunction->SetFunctionB(m_EnsembleRegressionEntropyFunction);
    }
    else if (mode == 4)
    {
      m_LinkingFunction->SetFunctionB(m_EnsembleMixedEffectsEntropyFunction);
    }
    else if (mode == 5)
    {
        m_LinkingFunction->SetFunctionB(m_MeshBasedGeneralEntropyGradientFunction);
        m_MeshBasedGeneralEntropyGradientFunction->UseEntropy();
    }
    else if (mode == 6)
    {
        m_LinkingFunction->SetFunctionB(m_MeshBasedGeneralEntropyGradientFunction);
        m_MeshBasedGeneralEntropyGradientFunction->UseMeanEnergy();
    }
    else
    {
      m_LinkingFunction->SetFunctionB(m_EnsembleMeanFunction);
    }
    
    if (m_LinkingFunction->GetCOn() == true) m_LinkingFunction->SetFunctionC(m_EnsembleNormalPenaltyFunction);

    m_CorrespondenceMode = mode;
  }

  void SetAttributeScales(const std::vector<double> &s)
  {
    m_GeneralEntropyGradientFunction->SetAttributeScales(s);
    m_MeshBasedGeneralEntropyGradientFunction->SetAttributeScales(s);
    m_MeshBasedGeneralMeanGradientFunction->SetAttributeScales(s);
    m_GeneralShapeMatrix->SetAttributeScales(s);
    m_GeneralShapeGradMatrix->SetAttributeScales(s);
  }

  void SetXYZ(unsigned int i, bool flag)
  {
      m_MeshBasedGeneralEntropyGradientFunction->SetXYZ(i, flag);
      m_GeneralShapeMatrix->SetXYZ(i, flag);
      m_GeneralShapeGradMatrix->SetXYZ(i, flag);
  }

  void SetNormals(int i, bool flag)
  {
      m_MeshBasedGeneralEntropyGradientFunction->SetNormals(i, flag);
      m_GeneralShapeMatrix->SetNormals(i, flag);
      m_GeneralShapeGradMatrix->SetNormals(i, flag);
  }

  void SetAttributesPerDomain(const std::vector<int> s)
  {
      std::vector<int> s1;
      if (s.size() == 0)
      {
          s1.resize(m_MeshBasedGeneralEntropyGradientFunction->GetDomainsPerShape());
          for (int i = 0; i < m_MeshBasedGeneralEntropyGradientFunction->GetDomainsPerShape(); i++)
              s1[i] = 0;
      }
      else
          s1 = s;
      this->Superclass::SetAttributesPerDomain(s1);
      m_MeshBasedGeneralEntropyGradientFunction->SetAttributesPerDomain(s1);
      m_MeshBasedGeneralMeanGradientFunction->SetAttributesPerDomain(s1);
      m_GeneralShapeMatrix->SetAttributesPerDomain(s1);
      m_GeneralShapeGradMatrix->SetAttributesPerDomain(s1);
  }

  void AddAttributeImage(int d,
                         typename ParticleFunctionBasedShapeSpaceData<float, Dimension>::ImageType *I)
  {
    m_FunctionShapeData->AddFunctionImage(d, I);
  }

  ParticleDualVectorFunction<Dimension> *GetLinkingFunction()
  { return m_LinkingFunction.GetPointer(); }
  ParticleEnsembleNormalPenaltyFunction<Dimension> *GetEnsembleNormalPenaltyFunction()
  { return m_EnsembleNormalPenaltyFunction.GetPointer(); }
  ParticleEnsembleMeanFunction<Dimension> *GetEnsembleMeanFunction()
  { return m_EnsembleMeanFunction.GetPointer(); }
  ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleEntropyFunction()
  { return m_EnsembleEntropyFunction.GetPointer(); }
  ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleRegressionEntropyFunction()
  { return m_EnsembleRegressionEntropyFunction.GetPointer(); }
  ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleMixedEffectsEntropyFunction()
  { return m_EnsembleMixedEffectsEntropyFunction.GetPointer(); }
  ParticleGeneralEntropyGradientFunction<Dimension> *GetGeneralEntropyGradientFunction()
  { return m_GeneralEntropyGradientFunction.GetPointer(); }
  ParticleMeshBasedGeneralEntropyGradientFunction<Dimension> *GetMeshBasedGeneralEntropyGradientFunction()
  { return m_MeshBasedGeneralEntropyGradientFunction.GetPointer(); }
  ParticleMeshBasedGeneralMeanGradientFunction<Dimension> *GetMeshBasedGeneralMeanGradientFunction()
  { return m_MeshBasedGeneralMeanGradientFunction.GetPointer(); }
  
  const ParticleDualVectorFunction<Dimension> *GetLinkingFunction() const
  { return m_LinkingFunction.GetPointer(); }
  const ParticleEnsembleMeanFunction<Dimension> *GetEnsembleMeanFunction() const
  { return m_EnsembleMeanFunction.GetPointer(); }
  const ParticleEnsembleNormalPenaltyFunction<Dimension> *GetEnsembleNormalPenaltyFunction() const
  { return m_EnsembleNormalPenaltyFunction.GetPointer(); }
  const ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleEntropyFunction() const
  { return m_EnsembleEntropyFunction.GetPointer(); }
  const ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleRegressionEntropyFunction() const
  { return m_EnsembleRegressionEntropyFunction.GetPointer(); }
  const ParticleEnsembleEntropyFunction<Dimension> *GetEnsembleMixedEffectsEntropyFunction() const
  { return m_EnsembleMixedEffectsEntropyFunction.GetPointer(); }
  const ParticleGeneralEntropyGradientFunction<Dimension> *GetGeneralEntropyGradientFunction() const
  { return m_GeneralEntropyGradientFunction.GetPointer(); }
  const ParticleMeshBasedGeneralEntropyGradientFunction<Dimension> *GetMeshBasedGeneralEntropyGradientFunction() const
  { return m_MeshBasedGeneralEntropyGradientFunction.GetPointer(); }
  const ParticleMeshBasedGeneralMeanGradientFunction<Dimension> *GetMeshBasedGeneralMeanGradientFunction() const
  { return m_MeshBasedGeneralMeanGradientFunction.GetPointer(); }
  
  virtual void AllocateDataCaches();

  void SetDomainsPerShape(int n)
  {
    Superclass::SetDomainsPerShape(n);
    m_LinearRegressionShapeMatrix->SetDomainsPerShape(n);
    m_MixedEffectsShapeMatrix->SetDomainsPerShape(n);
    m_ShapeMatrix->SetDomainsPerShape(n);
    m_EnsembleMeanFunction->SetDomainsPerShape(n);
    m_EnsembleNormalPenaltyFunction->SetDomainsPerShape(n);
    m_MeshBasedGeneralEntropyGradientFunction->SetDomainsPerShape(n);
    m_MeshBasedGeneralMeanGradientFunction->SetDomainsPerShape(n);
    m_GeneralShapeMatrix->SetDomainsPerShape(n);
    m_GeneralShapeGradMatrix->SetDomainsPerShape(n);
  }

  void SetTimeptsPerIndividual(int n)
  {
    m_MixedEffectsShapeMatrix->SetTimeptsPerIndividual(n);
  }

  int GetCorrespondenceMode() const
  { return m_CorrespondenceMode; }

  virtual void InitializeOptimizationFunctions();
  
protected:
  MaximumEntropyCorrespondenceSampler();
  virtual ~MaximumEntropyCorrespondenceSampler() {};

  void PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

  void GenerateData();
  
private:
  MaximumEntropyCorrespondenceSampler(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  int m_CorrespondenceMode;

  typename ParticleDualVectorFunction<Dimension>::Pointer m_LinkingFunction;

  typename ParticleEnsembleNormalPenaltyFunction<Dimension>::Pointer m_EnsembleNormalPenaltyFunction;
  typename ParticleEnsembleMeanFunction<Dimension>::Pointer m_EnsembleMeanFunction;
  typename ParticleEnsembleEntropyFunction<Dimension>::Pointer m_EnsembleEntropyFunction;
  typename ParticleEnsembleEntropyFunction<Dimension>::Pointer m_EnsembleRegressionEntropyFunction;
  typename ParticleEnsembleEntropyFunction<Dimension>::Pointer m_EnsembleMixedEffectsEntropyFunction;
  typename ParticleGeneralEntropyGradientFunction<Dimension>::Pointer m_GeneralEntropyGradientFunction;
  typename ParticleShapeMatrixAttribute<double, Dimension>::Pointer m_ShapeMatrix;
  typename ParticleFunctionBasedShapeSpaceData<float, Dimension>::Pointer m_FunctionShapeData;
  typename ParticleShapeLinearRegressionMatrixAttribute<double, Dimension>::Pointer m_LinearRegressionShapeMatrix;
  typename ParticleShapeMixedEffectsMatrixAttribute<double, Dimension>::Pointer m_MixedEffectsShapeMatrix;

  typename ParticleGeneralShapeMatrix<double, Dimension>::Pointer m_GeneralShapeMatrix;
  typename ParticleGeneralShapeGradientMatrix<double, Dimension>::Pointer m_GeneralShapeGradMatrix;

  typename ParticleMeshBasedGeneralEntropyGradientFunction<Dimension>::Pointer m_MeshBasedGeneralEntropyGradientFunction;
  typename ParticleMeshBasedGeneralMeanGradientFunction<Dimension>::Pointer m_MeshBasedGeneralMeanGradientFunction;
};

} // end namespace itk

#if ITK_TEMPLATE_EXPLICIT
#include "Templates/itkMaximumEntropyCorrespondenceSampler+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkMaximumEntropyCorrespondenceSampler.txx"
#endif

#endif
