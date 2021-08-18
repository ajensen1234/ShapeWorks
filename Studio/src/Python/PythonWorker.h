#pragma once

#include <QObject>

#include <Job/Job.h>
#include <Data/Session.h>
#include <ParticleShapeStatistics.h>

namespace shapeworks {
class Optimize;
class QGroom;
class QDeepSSM;
class OptimizeParameters;
class PythonLogger;



class PythonWorker : public QObject {
Q_OBJECT

public:
  enum class JobType {
    DeepSSM_SplitType, DeepSSM_AugmentationType, DeepSSM_TrainingType, DeepSSM_TestingType, Stats_Pvalues
  };

  PythonWorker();
  ~PythonWorker();

  void set_deep_ssm(QSharedPointer<QDeepSSM> deep_ssm);

  void set_stats(ParticleShapeStatistics stats);

  void run_job(JobType job);

  void run_job(QSharedPointer<Job> job);

  void incoming_python_message(std::string message_string);
  void incoming_python_progress(double value);

  void end_python();

  void abort_job();

public Q_SLOTS:

  bool init();

  void start_deepssm_augmentation();
  void start_deepssm_training();
  void start_deepssm_testing();
  void start_stats_pvalues();

  void start_job(QSharedPointer<Job> job);

  void finalize_python();

Q_SIGNALS:

  void deepssm_augmentation_complete();
  void deepssm_training_complete();
  void job_finished(shapeworks::PythonWorker::JobType);

  void result_ready();
  void error_message(QString);
  void warning_message(QString);
  void progress(double);
  void message(QString);
  void finished();

private:

  bool initialized_ = false;
  bool initialized_success_ = false;
  JobType current_job_;
  ParticleShapeStatistics stats_;

  void finish_job();

  QSharedPointer<QDeepSSM> deep_ssm_;
  QSharedPointer<PythonLogger> python_logger_;


  QSharedPointer<Job> current_jobber_;

  Eigen::MatrixXd group_pvalues_;
  QThread* thread_;
};
}

Q_DECLARE_METATYPE(shapeworks::PythonWorker::JobType);
